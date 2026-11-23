#include "Orbite_Recorder.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
namespace orbite {
const char *csvHeader(){return
 "time_ms;sequence;present_mask;valid_mask;"
 "ms_pressure_pa;ms_temperature_c;ms_altitude_m;ms_age_ms;ms_seq;"
 "bmp_pressure_pa;bmp_temperature_c;bmp_altitude_m;bmp_age_ms;bmp_seq;"
 "bmi_ax_ms2;bmi_ay_ms2;bmi_az_ms2;bmi_gx_dps;bmi_gy_dps;bmi_gz_dps;bmi_age_ms;bmi_seq;"
 "lsm_ax_ms2;lsm_ay_ms2;lsm_az_ms2;lsm_gx_dps;lsm_gy_dps;lsm_gz_dps;lsm_age_ms;lsm_seq;"
 "mag_x_gauss;mag_y_gauss;mag_z_gauss;mag_age_ms;mag_seq;"
 "gps_utc;gps_fix;gps_fix3d;gps_fix_type;gps_quality;gps_lat_deg;gps_lon_deg;gps_alt_m;gps_sats;gps_hdop;gps_age_ms;gps_rx_age_ms;gps_gsa_age_ms;gps_seq;"
 "battery_v;switches_closed;skipped_samples;sd_errors;radio_errors;tx_done;max_io_ms\n";}
// Bounded writer: never emit a truncated CSV row.
class Row {
 public:
  Row(char *p,size_t cap):p_(p),cap_(cap){}
  void add(const char *fmt,...){
    if(!ok_||used_>=cap_) {ok_=false;return;}
    va_list args;va_start(args,fmt);int n=vsnprintf(p_+used_,cap_-used_,fmt,args);va_end(args);
    if(n<0||size_t(n)>=cap_-used_){ok_=false;return;}used_+=n;
  }
  size_t size()const{return ok_?used_:0;}
 private:char*p_;size_t cap_,used_=0;bool ok_=true;
};
size_t formatCsv(const Snapshot&s,const RunStatus&r,char*p,size_t cap){
 if(!p||!cap)return 0;Row w(p,cap);
 w.add("%lu;%lu;%u;%u;",(unsigned long)s.timeMs,(unsigned long)s.sequence,s.presentMask,s.validMask);
 auto baro=[&](const Barometer&b){w.add("%.3f;%.3f;%.3f;%lu;%lu;",double(b.pressurePa),double(b.temperatureC),double(b.altitudeM),(unsigned long)b.ageMs,(unsigned long)b.sequence);};
 baro(s.ms);baro(s.bmp);
 auto imu=[&](const Imu&i){w.add("%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%lu;%lu;",double(i.ax),double(i.ay),double(i.az),double(i.gx),double(i.gy),double(i.gz),(unsigned long)i.ageMs,(unsigned long)i.sequence);};
 imu(s.bmi);imu(s.lsm);
 w.add("%.5f;%.5f;%.5f;%lu;%lu;",double(s.mag.x),double(s.mag.y),double(s.mag.z),(unsigned long)s.mag.ageMs,(unsigned long)s.mag.sequence);
 const Gps&g=s.gps;
 w.add("%s;%u;%u;%u;%u;%.7f;%.7f;%.3f;%u;%.2f;%lu;%lu;%lu;%lu;",g.utc,unsigned(g.fix),unsigned(g.fix3d),g.fixType,g.quality,g.latitude,g.longitude,double(g.altitudeM),g.satellites,double(g.hdop),(unsigned long)g.ageMs,(unsigned long)g.rxAgeMs,(unsigned long)g.gsaAgeMs,(unsigned long)g.sequence);
 w.add("%.3f;%u;%lu;%lu;%lu;%lu;%lu\n",double(s.batteryV),s.switches,(unsigned long)r.skippedSamples,(unsigned long)r.sdErrors,(unsigned long)r.radioErrors,(unsigned long)r.txDone,(unsigned long)r.maxIoMs);
 return w.size();
}
bool Recorder::fail(){
 ++errors_;ready_=false;used_=0;
 if(opened_){storage_->close();opened_=false;}
 return false;
}
bool Recorder::flush(){
 if(!ready_)return false;
 if(used_ && storage_->write(buffer_,used_)!=used_)return fail();
 used_=0;return true;
}
bool Recorder::add(const char*p,size_t n){
 if(!ready_||!p||n>sizeof buffer_)return false;
 if(used_+n>sizeof buffer_ && !flush())return false;
 memcpy(buffer_+used_,p,n);used_+=n;return true;
}
bool Recorder::begin(LogStorage&storage,const Snapshot&s){
 if(opened_ && !close())return false;
 storage_=&storage;ready_=false;used_=0;name_[0]=0;
 for(unsigned i=1;i<=9999;++i){
   char candidate[13];snprintf(candidate,sizeof candidate,"DATA%04u.CSV",i);
   OpenResult result=storage.createExclusive(candidate);
   if(result==OpenResult::Exists)continue;
   if(result!=OpenResult::Ok)return fail();
   strcpy(name_,candidate);opened_=ready_=true;break;
 }
 if(!ready_)return fail();
 lastSync_=s.timeMs;
 char meta[256];int n=snprintf(meta,sizeof meta,"# schema=orbite-passive-1\n# firmware=Orbite-0.3.0;mode=passive;clock=millis_u32\n# present_mask=%u;ms_ground_pa=%.3f;bmp_ground_pa=%.3f\n",s.presentMask,double(s.ms.groundPa),double(s.bmp.groundPa));
 if(n<=0||size_t(n)>=sizeof meta)return fail();
 if(!add(meta,n)||!add(csvHeader(),strlen(csvHeader()))||!flush())return false;
 if(!storage_->sync())return fail();return true;
}
bool Recorder::append(const Snapshot&s,const RunStatus&r){
 if(!ready_)return false;
 char line[2048];size_t n=formatCsv(s,r,line,sizeof line);
 if(!n)return fail();return add(line,n);
}
bool Recorder::service(uint32_t now){
 if(!ready_)return false;
 if(uint32_t(now-lastSync_)<500)return true;
 if(!flush())return false;
 if(!storage_->sync())return fail();lastSync_=now;return true;
}
bool Recorder::close(){
 if(!opened_)return ready_;
 if(!flush())return false;
 if(!storage_->sync())return fail();
 bool ok=storage_->close();opened_=false;ready_=false;
 if(!ok){++errors_;return false;}return true;
}
}
