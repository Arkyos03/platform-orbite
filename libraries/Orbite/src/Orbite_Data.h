/* Passive acquisition types. No Arduino dependency. MIT, ORBITE-I contributors. */
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <math.h>
namespace orbite {
enum Sensor : uint16_t { MS5611=1, BMP390=2, BMI088=4, LSM6DSOX=8, MAG=16, GPS=32, BATTERY=64 };
struct SampleStamp {
  uint32_t timeMs=0, sequence=0;
  bool seen=false;
  void mark(uint32_t now) { timeMs=now; seen=true; ++sequence; }
  uint32_t age(uint32_t now) const { return seen ? uint32_t(now-timeMs) : UINT32_MAX; }
  bool fresh(uint32_t now,uint32_t limit) const { return seen && age(now)<=limit; }
};
struct Barometer {
  float pressurePa=NAN, temperatureC=NAN, altitudeM=NAN, groundPa=NAN;
  uint32_t ageMs=UINT32_MAX, sequence=0;
};
struct Imu {
  float ax=NAN, ay=NAN, az=NAN, gx=NAN, gy=NAN, gz=NAN;
  uint32_t ageMs=UINT32_MAX, sequence=0;
};
struct Magnetometer {
  float x=NAN, y=NAN, z=NAN;
  uint32_t ageMs=UINT32_MAX, sequence=0;
};
struct Gps {
  double latitude=NAN, longitude=NAN;
  float altitudeM=NAN, hdop=NAN;
  uint32_t ageMs=UINT32_MAX, rxAgeMs=UINT32_MAX, gsaAgeMs=UINT32_MAX, sequence=0;
  uint8_t satellites=0, quality=0, fixType=0;
  bool hasPosition=false, fix=false, fix3d=false;
  char utc[11]="";
};
struct Snapshot {
  uint32_t timeMs=0, sequence=0;
  uint16_t presentMask=0, validMask=0;
  Barometer ms, bmp;
  Imu bmi, lsm;
  Magnetometer mag;
  Gps gps;
  float batteryV=NAN;
  uint8_t switches=0; // raw closed-input bits: launch, pin, aux (not arming state)
};
struct RunStatus {
  bool logging=false;
  uint32_t skippedSamples=0, sdErrors=0, radioErrors=0, txDone=0, maxIoMs=0;
};
// Preserve phase, skip missed deadlines; never generate catch-up bursts.
class Periodic {
 public:
  explicit Periodic(uint32_t period):period_(period) {}
  void reset(uint32_t now) { last_=now; skipped_=0; }
  bool due(uint32_t now) {
    uint32_t elapsed=now-last_;
    if (!period_ || elapsed<period_) return false;
    uint32_t n=elapsed/period_; last_+=n*period_; skipped_+=n-1; return true;
  }
  uint32_t skipped() const { return skipped_; }
 private: uint32_t period_, last_=0, skipped_=0;
};
} // namespace orbite
