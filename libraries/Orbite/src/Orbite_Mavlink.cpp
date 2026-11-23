#include "Orbite_Mavlink.h"
#include "mavlink/orbite_link.h"
namespace orbite {
static int32_t scaled(double v,double scale,int32_t lo,int32_t hi){
 if(!isfinite(v))return 0;
 double n=round(v*scale);if(n<lo)return lo;if(n>hi)return hi;return (int32_t)n;
}
size_t encodeDiagnostic(const Snapshot&s,const RunStatus&r,uint8_t*out,size_t cap,bool withStatus){
 if(!out || cap < (withStatus?80u:40u))return 0;
 mavlink_rocket_telemetry_t t={};mavlink_rocket_status_t status={};
 t.time_boot_ms=status.time_boot_ms=s.timeMs;
 float altitude=0,temp=0;unsigned count=0;
 if((s.validMask&MS5611)&&isfinite(s.ms.altitudeM)){altitude+=s.ms.altitudeM;temp+=s.ms.temperatureC;++count;}
 if((s.validMask&BMP390)&&isfinite(s.bmp.altitudeM)){altitude+=s.bmp.altitudeM;temp+=s.bmp.temperatureC;++count;}
 if(count){altitude/=count;temp/=count;}
 t.alt_agl=scaled(altitude,100,INT32_MIN,INT32_MAX);
 status.temperature=scaled(temp,100,INT16_MIN,INT16_MAX);
 status.vbat=scaled(s.batteryV,1000,0,UINT16_MAX);
 bool fix3d=(s.validMask&GPS) && s.gps.fix && s.gps.fix3d && s.gps.ageMs<=2000 && s.gps.gsaAgeMs<=2000 &&
     isfinite(s.gps.latitude)&&isfinite(s.gps.longitude)&&isfinite(s.gps.altitudeM);
 if(fix3d){
  t.flags|=ROCKET_TLM_FLAGS_GPS_FIX;
  t.lat=scaled(s.gps.latitude,1e7,-900000000,900000000);
  t.lon=scaled(s.gps.longitude,1e7,-1800000000,1800000000);
  t.alt_gps=scaled(s.gps.altitudeM,1,INT16_MIN,INT16_MAX);
 }
 status.gps_fix_type=fix3d?3:(s.gps.fix&&s.gps.ageMs<=2000&&s.gps.gsaAgeMs<=2000&&s.gps.fixType==2?2:1);
 if(s.gps.rxAgeMs==UINT32_MAX)status.gps_fix_type=0;
 if(s.gps.rxAgeMs<=2000)status.gps_sats=s.gps.satellites;
 status.gps_hdop=(s.gps.fix&&s.gps.ageMs<=2000&&isfinite(s.gps.hdop))?scaled(s.gps.hdop,100,0,UINT16_MAX):UINT16_MAX;
 if(s.validMask&MS5611)status.sensor_health|=ROCKET_SENSOR_BARO_1;
 if(s.validMask&BMP390)status.sensor_health|=ROCKET_SENSOR_BARO_2;
 if(s.validMask&(BMI088|LSM6DSOX))status.sensor_health|=ROCKET_SENSOR_IMU;
 if(fix3d)status.sensor_health|=ROCKET_SENSOR_GPS;
 if(r.logging){t.flags|=ROCKET_TLM_FLAGS_LOGGING;status.sensor_health|=ROCKET_SENSOR_SD_CARD;}
 if(isfinite(s.batteryV)&&s.batteryV<10.5f)t.flags|=ROCKET_TLM_FLAGS_LOW_BATTERY;
 const uint16_t required=MS5611|BMP390|BMI088|LSM6DSOX|MAG|BATTERY;
 bool fault=(s.validMask&required)!=required || r.radioErrors>0;
 if(fault)t.flags|=ROCKET_TLM_FLAGS_SENSOR_FAULT;
 t.flight_state=fault?ROCKET_FLIGHT_STATE_FAULT:ROCKET_FLIGHT_STATE_INIT;
 // Orientation, axial acceleration, flight phases, deployment, maxima: unavailable
 // in passive mode. Zero is the legacy dialect's neutral value, NOT a measurement.
 return orbite_build_packet(out,(uint16_t)(cap>255?255:cap),&t,withStatus?&status:nullptr);
}
uint32_t loraAirtimeMs(uint8_t len,uint8_t sf,uint32_t bw,uint16_t preamble){
 if(sf<5||sf>12||!bw)return 0;
 // SX126x explicit header, CRC enabled, CR 4/5. Semtech sx126x_driver reference.
 bool ldro=(uint64_t(1u<<sf)*1000/bw)>16;
 int bits=8*len+16-4*sf+20+(sf>6?8:0);
 int block=4*(sf-((sf>6&&ldro)?2:0));
 unsigned coded=bits>0?unsigned((bits+block-1)/block)*5:0;
 unsigned quarters=4*(coded+preamble+12+(sf<=6?2:0))+1;
 uint64_t us=uint64_t(quarters)*(1u<<(sf-2))*1000;
 return (us+bw-1)/bw;
}
}
