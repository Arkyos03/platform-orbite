/*
 * Orbite - library of the ORBITE-I flight computer (CORE-I + BEACON-I + SPARK-I + REACTOR-I)
 * Copyright (c) 2026 Arkyos03 - MIT License
 *
 *   #include <Orbite.h>
 *
 *   void setup() {
 *     Orbite.begin();              // pins, sensors, GPS, ground altitude
 *     Orbite.selfTest();           // 3 beeps = everything OK
 *     Orbite.logBegin();           // new file VOL_XXX.CSV on the micro SD card
 *   }
 *   void loop() {
 *     Orbite.update();             // read the sensors, follow the flight
 *     Orbite.logFlight();          // one line of flight data
 *     if (Orbite.apogee()) Orbite.fire(1);   // pyro channel 1 at apogee
 *   }
 *
 * Pin names (FIRE_1, SERVO_1, CS_BMP390...) come from the board package:
 * Tools > Board > ORBITE-I > ORBITE-I CORE.
 */
#ifndef ORBITE_LIB_H
#define ORBITE_LIB_H

#include <Arduino.h>
#include <SPI.h>
#include "Orbite_Data.h"
#include "Orbite_Baro.h"
#include "Orbite_Nmea.h"

#if !defined(ARDUINO_ORBITE_CORE)
#error "Select Tools > Board > ORBITE-I > ORBITE-I CORE (STM32F405)"
#endif

/* Flight phases returned by Orbite.state() */
enum OrbiteFlightState : uint8_t {
  ORBITE_GROUND  = 0,   /* on the launch pad                 */
  ORBITE_ASCENT  = 1,   /* liftoff detected                  */
  ORBITE_DESCENT = 2,   /* apogee detected                   */
  ORBITE_LANDED  = 3,   /* back on the ground after the flight */
};

/* Error codes of Orbite.selfTest() (number of short beeps after a long one) */
enum OrbiteError : uint8_t {
  ORBITE_OK          = 0,
  ORBITE_ERR_BARO    = 1,  /* no barometer answers (MS5611 and BMP390)   */
  ORBITE_ERR_IMU     = 2,  /* no IMU answers (BMI088 and LSM6DSOX)       */
  ORBITE_ERR_SD      = 3,  /* micro SD card missing / unreadable         */
  ORBITE_ERR_BATTERY = 4,  /* battery too low                            */
  ORBITE_ERR_PYRO    = 5,  /* armed, but a requested pyro channel is open */
};

/* LoRa telemetry frame (38 bytes), sent by telemetrySend(), decoded by telemetryReceive() */
#define ORBITE_TELEM_MAGIC 0x4F52   /* "OR" */
struct __attribute__((packed)) OrbiteTelemetry {
  uint16_t magic;
  uint16_t counter;       /* frame number                              */
  uint32_t time_ms;       /* millis() of the flight computer           */
  uint8_t  state;         /* OrbiteFlightState                         */
  int32_t  altitude_dm;   /* altitude above the ground, 0.1 m          */
  int32_t  maxAlt_dm;     /* max altitude, 0.1 m                       */
  int16_t  vspeed_cms;    /* vertical speed, cm/s                      */
  int16_t  accel_cg;      /* total acceleration, 0.01 g                */
  int32_t  lat_e7;        /* GPS latitude, 1e-7 degree                 */
  int32_t  lon_e7;        /* GPS longitude, 1e-7 degree                */
  int16_t  gpsAlt_m;      /* GPS altitude above sea level, m           */
  uint8_t  gpsSats;       /* bit 7 = fix, bits 0..6 = satellites       */
  uint16_t vbat_cv;       /* battery, 0.01 V                           */
  uint8_t  pyro;          /* bits 0..3 = fired, bits 4..7 = continuity */
  uint8_t  flags;         /* bit 0 = pyro armed, 1 = SD log, 2 = pin removed */
  uint16_t crc;           /* CRC-16 CCITT of the previous bytes        */
};

/* Extra SPI buses of the board (SPI = SPI1 is provided by the core) */
extern SPIClass OrbiteSPI2;   /* BMP390, LSM6DSOX, MMC5983MA (BEACON-I) */
extern SPIClass OrbiteSPI3;   /* LoRa E22 (BEACON-I), flash W25Q128      */

class OrbiteBoard {
public:
  /* ============================================================= start-up */
  /* Configures every pin (pyro outputs LOW, all chip selects HIGH), starts the
   * sensors and the GPS, measures the ground altitude.
   * Returns false if a sensor did not answer (the board still works). */
  bool begin();
  // Passive sensor service. Call first after reset; never runs the flight sequencer.
  bool beginPassive();
  orbite::Snapshot snapshot(uint32_t sequence = 0) const;
  uint32_t flashIdentity(); // JEDEC read only; no erase/program


  /* Reads the sensors, the GPS, follows the flight, stops the pyro pulses.
   * Call it as often as possible in loop(). */
  void update();

  /* Checks barometers, IMUs, SD card, battery and the pyro channels listed in
   * pyroMask (bit 0 = channel 1...), prints a report on Serial and beeps:
   * 3 short beeps = OK, 1 long beep + N short beeps = error code N. */
  OrbiteError selfTest(bool needSdCard = true, bool needBattery = true, uint8_t pyroMask = 0);

  /* ============================================================= pyro (SPARK-I) */
  /* Fires channel n (1..4) for duration_ms (default 1 s). Non-blocking: the
   * output is switched off by update(). The SPARK-I rail must be armed (RBF pin). */
  void  fire(uint8_t n, uint32_t duration_ms = 1000);
  void  fireStop(uint8_t n);                    /* immediate stop, n = 0 : all channels */
  bool  firing(uint8_t n);                      /* channel n is on right now */
  bool  fired(uint8_t n);                       /* channel n has been fired since begin() */
  float pyroVoltage();                          /* V, pyro rail (0 when disarmed) */
  bool  pyroArmed(float threshold = 3.0f);      /* rail powered */
  float continuityVoltage(uint8_t n);           /* V seen through the e-match of channel n */
  /* true if an e-match is connected on channel n. Only meaningful when armed. */
  bool  continuity(uint8_t n);

  /* ============================================================= inputs */
  bool launchSwitch();                          /* SW_LAUNCH closed */
  bool pinRemoved();                            /* SW_PIN open (RBF pin pulled) */
  bool auxSwitch();                             /* SW_AUX closed */

  /* ============================================================= buzzer / LED */
  void beep(uint32_t frequency = 4000, uint32_t duration_ms = 100);
  void beepBlocking(uint32_t frequency = 4000, uint32_t duration_ms = 100);
  void beepCount(uint8_t count, uint32_t frequency = 4000);
  /* Beeps an altitude digit by digit (default : max altitude). 0 = long beep. */
  void beepAltitude(float meters = -1.0f);
  /* Beeps the continuity of the 4 pyro channels : 1 high beep = connected,
   * 1 low beep = open, for channels 1 to 4. */
  void beepContinuity();

  void rgb(uint8_t r, uint8_t g, uint8_t b);    /* SK6805 colour, stops the status animation */
  void rgb(uint32_t rgb);                       /* 0xRRGGBB */
  void statusLed(bool on);                      /* automatic status colours (on by default) */
  void statusBrightness(uint8_t level);         /* 0..255, default 40 */

  /* ============================================================= battery */
  float batteryVoltage();                       /* V */
  bool  lowBattery(float threshold = 10.5f);    /* 3S LiPo : 3.5 V / cell */

  /* ============================================================= barometers
   * MS5611 (SPI1) and BMP390 (SPI2) : altitude() is the mean of the ones that answer. */
  float pressure();                             /* Pa (mean) */
  float temperature();                          /* °C (BMP390 if present) */
  float altitude();                             /* m above the ground (setGroundLevel) */
  float altitudeSea(float seaLevel_hPa = 1013.25f);
  void  setGroundLevel();                       /* altitude() = 0 here (done by begin) */
  float verticalSpeed();                        /* m/s, > 0 going up (filtered) */
  float altitudeMS5611();                       /* m above the ground, NAN if absent */
  float altitudeBMP390();
  bool  ms5611OK() { return _msOK; }           /* MS5611 or MS5607 */
  /* MS5611 and MS5607 share the pinout and the SPI commands, not the compensation formulas.
   * MS56XX_AUTO (default) : at begin(), the formula that agrees with the BMP390/388 is kept
   * (MS5611 if the BMP is absent). Force the part with setMs56xx() BEFORE begin(). */
  void  setMs56xx(orbite::Ms56xx model) { _msModel = model; }
  orbite::Ms56xx ms56xx() { return _msModel; }
  const char *ms56xxName() { return _msModel == orbite::MS56XX_5607 ? "MS5607" : "MS5611"; }
  bool  bmp390OK() { return _bmpOK; }          /* BMP390 or BMP388 */

  /* ============================================================= IMUs
   * BMI088 (SPI1, ±24 g, ±2000 °/s) is used first, LSM6DSOX (SPI2, ±16 g) otherwise. */
  void  accel(float &x, float &y, float &z);    /* m/s² */
  void  gyro(float &x, float &y, float &z);     /* °/s  */
  float accelX() { return _ax; }
  float accelY() { return _ay; }
  float accelZ() { return _az; }
  float gyroX()  { return _gx; }
  float gyroY()  { return _gy; }
  float gyroZ()  { return _gz; }
  float gForce();                               /* total acceleration in g (1 at rest) */
  void  accelLSM(float &x, float &y, float &z); /* LSM6DSOX alone, m/s² */
  void  gyroLSM(float &x, float &y, float &z);  /* LSM6DSOX alone, °/s  */
  bool  bmi088OK()   { return _bmiOK; }
  bool  lsm6dsoxOK() { return _lsmOK; }

  /* ============================================================= magnetometer (MMC5983MA, BEACON-I) */
  void  mag(float &x, float &y, float &z);      /* gauss */
  float heading();                              /* degrees 0..360, board flat */
  bool  magOK() { return _magOK; }

  /* ============================================================= GPS (MAX-M10S, BEACON-I, Serial2) */
  bool     gpsFix()       { return _gpsFix; }
  double   gpsLatitude()  { return _gpsLat; }   /* degrees, > 0 north */
  double   gpsLongitude() { return _gpsLon; }   /* degrees, > 0 east  */
  float    gpsAltitude()  { return _gpsAlt; }   /* m above sea level */
  uint8_t  gpsSatellites(){ return _gpsSats; }
  float    gpsHdop()      { return _gpsHdop; }
  uint32_t gpsBaud()      { return _gpsBaud; }   /* vitesse du GPS trouvée par begin() (0 = muet) */
  uint32_t gpsAge();                            /* ms since the last position */
  const char *gpsTime()   { return _gpsTime; }  /* "hhmmss" UTC */

  /* ============================================================= flight */
  bool     liftoff();
  bool     apogee();
  bool     landed();
  OrbiteFlightState state() { update(); return _state; }
  uint32_t flightTime();                        /* ms since liftoff */
  float    maxAltitude();
  float    maxGForce() { return _maxG; }
  void     resetFlight();
  void setApogeeDrop(float meters)   { _apogeeDrop = meters; }   /* default 3 m      */
  void setApogeeMinTime(uint32_t ms) { _apogeeMinTime = ms; }    /* default 1500 ms  */
  void setApogeeTimeout(uint32_t ms) { _apogeeTimeout = ms; }    /* default 0 = off  */
  void setLiftoffAltitude(float m)   { _liftoffAlt = m; }        /* default 15 m     */
  void setLiftoffAccel(float g)      { _liftoffG = g; }          /* default 3 g      */
  void setLandingTime(uint32_t ms)   { _landingTime = ms; }      /* default 3000 ms  */

  /* ============================================================= servos (REACTOR-I, 50 Hz hardware PWM) */
  void servoAngles(uint8_t n, int closedAngle, int openAngle);  /* default 0 / 90 */
  void servoOpen(uint8_t n);
  void servoClose(uint8_t n);
  void servoWrite(uint8_t n, int angle);        /* 0..180 */
  void servoMicroseconds(uint8_t n, uint16_t us);  /* 500..2500 */
  void servoDetach(uint8_t n);

  /* ============================================================= micro SD card (SDIO) */
  bool sdCardPresent();                         /* SD_DETECT */
  /* Opens a new file VOL_001.CSV, VOL_002.CSV... (never overwrites a flight). */
  bool logBegin(const char *prefix = "VOL", const char *header = nullptr);
  bool logReady() { return _logOK; }
  const char *logFileName() { return _logName; }
  void setLogSeparator(char c) { _sep = c; }    /* default ';' */
  template <typename... T> void log(T... values)
  {
    if (!_logOK) return;
    logStart();
    logValues(values...);
    logEnd();
  }
  void logFlight();                             /* time, state, altitude, speed, baros, IMU, GPS, battery, pyro */
  void logText(const char *text);
  void logFlush();
  void logClose();
  void printFlight(Print &out = Serial);
  void printFlightHeader(Print &out = Serial);

  /* ============================================================= LoRa telemetry (E22-900M22S, BEACON-I)
   * Default : 869.525 MHz (869.4-869.65 MHz sub-band : 500 mW max, 10 % duty cycle),
   * 22 dBm, SF7, 250 kHz -> about 40 ms per telemetry frame. */
  bool     loraBegin(float frequencyMHz = 869.525f, int8_t powerDbm = 22, uint8_t spreadingFactor = 7,
                     uint32_t bandwidthHz = 250000, uint16_t preamble = 8);
  bool loraBeginStation(); // MAVLink station profile: SF6, preamble 12
  uint32_t loraErrorCount() const { return _loraErrors; }
  bool     loraOK() { return _loraOK; }
  bool     loraSend(const uint8_t *data, uint8_t len);   /* non-blocking, false if busy */
  bool     loraBusy();                                    /* a packet is being sent */
  void     loraListen();                                  /* receive continuously (ground station) */
  bool     loraAvailable();
  uint8_t  loraRead(uint8_t *buf, uint8_t maxLen);
  float    loraRSSI() { return _loraRssi; }               /* dBm, last packet received */
  float    loraSNR()  { return _loraSnr; }                /* dB  */
  uint32_t loraTimeOnAir(uint8_t len);                    /* ms */
  uint32_t loraSentCount() { return _loraSent; }
  bool     telemetrySend();                               /* one OrbiteTelemetry frame */
  void     telemetryFill(OrbiteTelemetry &t);
  bool     telemetryReceive(OrbiteTelemetry &t);          /* true if a valid frame arrived */
  void     printTelemetry(const OrbiteTelemetry &t, Print &out = Serial);
  void     printTelemetryHeader(Print &out = Serial);
  void     logTelemetry(const OrbiteTelemetry &t);        /* one CSV line in the log file */

  /* ============================================================= misc */
  void enterBootloader();                       /* reboot into the UF2 bootloader (ORBITEBOOT) */

private:
  /* sensors */
  void csHigh();
  void acquire();
  bool msBegin();   void msTask();
  bool bmpBegin();  bool bmpRead();
  bool bmiBegin();  void bmiRead();
  bool lsmBegin();  void lsmRead();
  bool magBegin();  void magTask();
  void gpsBegin();  void gpsTask(); void gpsLine(char *line);
  void loraTask();
  void baroUpdate();
  void flightUpdate();
  void pyroUpdate();
  void ledUpdate();
  void ledShow(uint8_t r, uint8_t g, uint8_t b);
  /* logging */
  void logStart();
  void logEnd();
  void logValue(float v);
  void logValue(double v);
  void logValue(int v);
  void logValue(long v)          { logValue((int)v); }
  void logValue(unsigned int v);
  void logValue(unsigned long v) { logValue((unsigned int)v); }
  void logValue(bool v)          { logValue((int)v); }
  void logValue(const char *v);
  void logValue(const String &v) { logValue(v.c_str()); }
  void logValues() {}
  template <typename F, typename... R> void logValues(F first, R... rest)
  {
    logValue(first);
    logValues(rest...);
  }
  void flightLine(Print &out);

  bool _begun = false;
  bool _passive = false;
  orbite::NmeaParser _nmea;
  orbite::SampleStamp _msStamp, _bmpStamp, _bmiStamp, _lsmStamp, _magStamp, _batteryStamp;
  float _sampleBattery = NAN;
  uint8_t _sampleSwitches = 0;

  bool _msOK = false, _bmpOK = false, _bmiOK = false, _lsmOK = false, _magOK = false;

  /* MS5611 */
  uint16_t _msC[8];
  orbite::Ms56xx _msModel = orbite::MS56XX_AUTO;
  uint8_t  _msStep = 0;
  uint32_t _msT0 = 0, _msD1 = 0, _msD2 = 0;
  float _msP = NAN, _msT = NAN, _msGround = NAN;
  /* BMP390 */
  double _parT1, _parT2, _parT3;
  double _parP1, _parP2, _parP3, _parP4, _parP5, _parP6, _parP7, _parP8, _parP9, _parP10, _parP11;
  float _bmpP = NAN, _bmpT = NAN, _bmpGround = NAN;
  uint32_t _bmpLast = 0;
  /* fused */
  float _alt = 0, _vSpeed = 0, _lastAlt = 0;
  uint32_t _lastAltMs = 0;
  /* IMU */
  float _ax = 0, _ay = 0, _az = 0, _gx = 0, _gy = 0, _gz = 0;
  float _lax = 0, _lay = 0, _laz = 0, _lgx = 0, _lgy = 0, _lgz = 0;
  uint32_t _imuLast = 0;
  /* magnetometer */
  float _mx = 0, _my = 0, _mz = 0;
  float _magOff[3] = {0, 0, 0}, _magSet[3] = {0, 0, 0};   /* MMC5983MA bridge offset, SET reading */
  uint8_t _magStep = 0, _magCount = 0;
  uint32_t _magT0 = 0;
  /* GPS */
  char _gpsBuf[100];
  uint8_t _gpsLen = 0;
  bool _gpsFix = false;
  double _gpsLat = 0, _gpsLon = 0;
  float _gpsAlt = 0, _gpsHdop = 99;
  uint32_t _gpsBaud = 0;
  uint8_t _gpsSats = 0;
  uint32_t _gpsMs = 0;
  char _gpsTime[7] = "000000";
  /* pyro */
  uint32_t _fireEnd[4] = {0, 0, 0, 0};
  bool _firing[4] = {false, false, false, false};
  bool _fired[4] = {false, false, false, false};
  /* flight */
  OrbiteFlightState _state = ORBITE_GROUND;
  uint32_t _liftoffMs = 0, _apogeeMs = 0, _stillMs = 0;
  float _maxAlt = 0, _maxG = 0, _stillAlt = 0;
  float _apogeeDrop = 3.0f, _liftoffAlt = 15.0f, _liftoffG = 3.0f;
  uint32_t _apogeeMinTime = 1500, _apogeeTimeout = 0, _landingTime = 3000;
  /* servos */
  int16_t _servoClosed[4] = {0, 0, 0, 0}, _servoOpen[4] = {90, 90, 90, 90};
  bool _servoOn[4] = {false, false, false, false};
  /* LED */
  bool _statusLed = true;
  uint8_t _brightness = 40;
  uint32_t _ledLast = 0;
  /* LoRa */
  bool _loraOK = false, _loraTx = false, _loraRx = false, _loraListen = false;
  uint8_t _loraSF = 7;
  uint32_t _loraBW = 250000, _loraTxStart = 0, _loraSent = 0, _loraErrors = 0;
  uint8_t _loraBuf[255];
  uint16_t _loraPreamble = 8;
  uint32_t _loraTxTimeout = 2000;
  uint8_t _loraLen = 0;
  float _loraRssi = 0, _loraSnr = 0;
  uint16_t _telemCounter = 0;
  /* log */
  bool _logOK = false;
  char _logName[16] = "";
  char _sep = ';';
  bool _logFirst = true;
  uint32_t _logLastFlush = 0;
};

extern OrbiteBoard Orbite;

#endif
