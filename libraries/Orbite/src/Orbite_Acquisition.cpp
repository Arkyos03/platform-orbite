#include "Orbite.h"

bool OrbiteBoard::beginPassive()
{
  if (_begun) return _passive; // cannot switch modes mid-session
  _passive = true;
  _statusLed = false;
  return begin();
}

orbite::Snapshot OrbiteBoard::snapshot(uint32_t sequence) const
{
  using namespace orbite;
  Snapshot s;
  s.timeMs = millis(); s.sequence = sequence;
  s.presentMask = (_msOK ? MS5611 : 0) | (_bmpOK ? BMP390 : 0) |
      (_bmiOK ? BMI088 : 0) | (_lsmOK ? LSM6DSOX : 0) | (_magOK ? MAG : 0);
  auto baro = [&](Barometer &b, const SampleStamp &stamp, bool initialized, uint16_t bit,
                  float p, float t, float ref) {
    b.ageMs=stamp.age(s.timeMs); b.sequence=stamp.sequence; b.groundPa=ref;
    if (initialized && stamp.fresh(s.timeMs,200) && isfinite(p) && p>0 && isfinite(t)) {
      b.pressurePa=p; b.temperatureC=t;
      if(isfinite(ref) && ref>0)b.altitudeM=44330.0f*(1.0f-powf(p/ref,0.190295f));
      s.validMask|=bit;
    }
  };
  baro(s.ms,_msStamp,_msOK,MS5611,_msP,_msT,_msGround);
  baro(s.bmp,_bmpStamp,_bmpOK,BMP390,_bmpP,_bmpT,_bmpGround);
  auto imu = [&](Imu &v,const SampleStamp &stamp,bool initialized,uint16_t bit,
                 float ax,float ay,float az,float gx,float gy,float gz) {
    v.ageMs=stamp.age(s.timeMs); v.sequence=stamp.sequence;
    if(initialized && stamp.fresh(s.timeMs,100)) {
      v.ax=ax; v.ay=ay; v.az=az; v.gx=gx; v.gy=gy; v.gz=gz; s.validMask|=bit;
    }
  };
  imu(s.bmi,_bmiStamp,_bmiOK,BMI088,_ax,_ay,_az,_gx,_gy,_gz);
  imu(s.lsm,_lsmStamp,_lsmOK,LSM6DSOX,_lax,_lay,_laz,_lgx,_lgy,_lgz);
  s.mag.ageMs=_magStamp.age(s.timeMs); s.mag.sequence=_magStamp.sequence;
  if(_magOK && _magStamp.fresh(s.timeMs,200)) {
    s.mag.x=_mx; s.mag.y=_my; s.mag.z=_mz; s.validMask|=MAG;
  }
  s.gps=_nmea.snapshot(s.timeMs);
  if(s.gps.rxAgeMs!=UINT32_MAX)s.presentMask|=GPS;
  if(s.gps.fix)s.validMask|=GPS;
  if(_batteryStamp.seen)s.presentMask|=BATTERY;
  if(_batteryStamp.fresh(s.timeMs,200) && isfinite(_sampleBattery)) {
    s.batteryV=_sampleBattery; s.validMask|=BATTERY;
  }
  s.switches=_sampleSwitches;
  return s;
}

uint32_t OrbiteBoard::flashIdentity()
{
  if (!_begun) return 0;
  OrbiteSPI3.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS_FLASH, LOW);
  OrbiteSPI3.transfer(0x9F);
  uint32_t id=0;
  for(uint8_t i=0;i<3;++i)id=(id<<8)|OrbiteSPI3.transfer(0);
  digitalWrite(CS_FLASH,HIGH);
  OrbiteSPI3.endTransaction();
  return id;
}
