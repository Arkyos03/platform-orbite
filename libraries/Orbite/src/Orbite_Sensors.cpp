/*
 * Orbite - sensor drivers : MS5611, BMP390 (barometers), BMI088, LSM6DSOX (IMUs),
 * MMC5983MA (magnetometer)
 * Copyright (c) 2026 Arkyos03 - MIT License
 */
#include "Orbite.h"

static const SPISettings SPI_MS(10000000, MSBFIRST, SPI_MODE0);
static const SPISettings SPI_BMP(8000000, MSBFIRST, SPI_MODE0);
static const SPISettings SPI_BMI(8000000, MSBFIRST, SPI_MODE0);
static const SPISettings SPI_LSM(8000000, MSBFIRST, SPI_MODE0);
static const SPISettings SPI_MAG(8000000, MSBFIRST, SPI_MODE0);

/* ----------------------------------------------------------------- generic SPI register access */
/* dummy = true : the chip sends one dummy byte before the data (BMP390, BMI088 accel) */
static void spiRead(SPIClass &bus, const SPISettings &s, uint8_t cs, uint8_t reg,
                    uint8_t *buf, uint8_t len, bool dummy = false)
{
  bus.beginTransaction(s);
  digitalWrite(cs, LOW);
  bus.transfer(reg | 0x80);
  if (dummy) bus.transfer(0x00);
  for (uint8_t i = 0; i < len; i++) buf[i] = bus.transfer(0x00);
  digitalWrite(cs, HIGH);
  bus.endTransaction();
}

static uint8_t spiRead8(SPIClass &bus, const SPISettings &s, uint8_t cs, uint8_t reg, bool dummy = false)
{
  uint8_t v = 0;
  spiRead(bus, s, cs, reg, &v, 1, dummy);
  return v;
}

static void spiWrite8(SPIClass &bus, const SPISettings &s, uint8_t cs, uint8_t reg, uint8_t val)
{
  bus.beginTransaction(s);
  digitalWrite(cs, LOW);
  bus.transfer(reg & 0x7F);
  bus.transfer(val);
  digitalWrite(cs, HIGH);
  bus.endTransaction();
}

static float pressureToAltitude(float p, float p0)
{
  return 44330.0f * (1.0f - powf(p / p0, 0.190295f));
}

/* ===================================================================== MS5611 (SPI1) */

static void msCommand(uint8_t cmd)
{
  SPI.beginTransaction(SPI_MS);
  digitalWrite(CS_MS5611, LOW);
  SPI.transfer(cmd);
  digitalWrite(CS_MS5611, HIGH);
  SPI.endTransaction();
}

static uint32_t msRead(uint8_t cmd, uint8_t len)
{
  uint32_t v = 0;
  SPI.beginTransaction(SPI_MS);
  digitalWrite(CS_MS5611, LOW);
  SPI.transfer(cmd);
  for (uint8_t i = 0; i < len; i++) v = (v << 8) | SPI.transfer(0x00);
  digitalWrite(CS_MS5611, HIGH);
  SPI.endTransaction();
  return v;
}

bool OrbiteBoard::msBegin()
{
  msCommand(0x1E);                 /* reset */
  delay(5);
  bool allZero = true, allOnes = true;
  for (uint8_t i = 0; i < 8; i++) {
    _msC[i] = (uint16_t)msRead(0xA0 + i * 2, 2);
    if (_msC[i] != 0x0000) allZero = false;
    if (_msC[i] != 0xFFFF) allOnes = false;
  }
  if (allZero || allOnes) return false;
  /* CRC-4 of the PROM (AN520) */
  uint16_t prom[8];
  memcpy(prom, _msC, sizeof(prom));
  uint8_t crcRead = prom[7] & 0x000F;
  prom[7] &= 0xFF00;
  uint16_t rem = 0;
  for (uint8_t cnt = 0; cnt < 16; cnt++) {
    rem ^= (cnt & 1) ? (prom[cnt >> 1] & 0x00FF) : (prom[cnt >> 1] >> 8);
    for (uint8_t b = 8; b > 0; b--) rem = (rem & 0x8000) ? (rem << 1) ^ 0x3000 : (rem << 1);
  }
  rem = (rem >> 12) & 0x000F;
  if (rem != crcRead) return false;

  /* first measurement, blocking */
  msCommand(0x58); delay(10); _msD2 = msRead(0x00, 3);   /* temperature OSR 4096 */
  msCommand(0x48); delay(10); _msD1 = msRead(0x00, 3);   /* pressure    OSR 4096 */
  _msStep = 0;
  _msT0 = millis();
  msCommand(0x58);
  return true;
}

/* non-blocking : temperature, pressure, temperature... (OSR 4096 = 9.04 ms each) */
void OrbiteBoard::msTask()
{
  if (!_msOK || millis() - _msT0 < 10) return;
  if (_msStep == 0) {
    _msD2 = msRead(0x00, 3);
    msCommand(0x48);
    _msStep = 1;
  } else {
    _msD1 = msRead(0x00, 3);
    msCommand(0x58);
    _msStep = 0;
    /* compensation with second order (datasheets MS5611-01BA03 / MS5607-02BA03) */
    int32_t p, temp;
    if (orbite::ms56xxCompensate(_msModel, _msC, _msD1, _msD2, p, temp)) {
      _msP = (float)p;              /* Pa */
      _msT = temp / 100.0f;
      _msStamp.mark(millis());
    }
  }
  _msT0 = millis();
}

/* ===================================================================== BMP390 (SPI2) */

/* CHIP_ID : BMP390 = 0x60, BMP388 = 0x50 (same registers, same compensation) */
static bool bmpId(uint8_t id) { return id == 0x60 || id == 0x50; }

bool OrbiteBoard::bmpBegin()
{
  spiRead8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x00, true);       /* first access : wake up SPI */
  if (!bmpId(spiRead8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x00, true))) return false;
  spiWrite8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x7E, 0xB6);      /* soft reset */
  delay(10);

  uint8_t c[21];
  spiRead(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x31, c, 21, true);
  _parT1 = (double)(uint16_t)(c[0] | (c[1] << 8)) / pow(2, -8);
  _parT2 = (double)(uint16_t)(c[2] | (c[3] << 8)) / pow(2, 30);
  _parT3 = (double)(int8_t)c[4] / pow(2, 48);
  _parP1 = ((double)(int16_t)(c[5] | (c[6] << 8)) - 16384.0) / pow(2, 20);
  _parP2 = ((double)(int16_t)(c[7] | (c[8] << 8)) - 16384.0) / pow(2, 29);
  _parP3 = (double)(int8_t)c[9] / pow(2, 32);
  _parP4 = (double)(int8_t)c[10] / pow(2, 37);
  _parP5 = (double)(uint16_t)(c[11] | (c[12] << 8)) / pow(2, -3);
  _parP6 = (double)(uint16_t)(c[13] | (c[14] << 8)) / pow(2, 6);
  _parP7 = (double)(int8_t)c[15] / pow(2, 8);
  _parP8 = (double)(int8_t)c[16] / pow(2, 15);
  _parP9 = (double)(int16_t)(c[17] | (c[18] << 8)) / pow(2, 48);
  _parP10 = (double)(int8_t)c[19] / pow(2, 48);
  _parP11 = (double)(int8_t)c[20] / pow(2, 65);

  spiWrite8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x1C, 0x03);      /* OSR : pressure x8, temperature x1 */
  spiWrite8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x1D, 0x02);      /* ODR 50 Hz */
  spiWrite8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x1F, 0x04);      /* IIR coefficient 3 */
  spiWrite8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x1B, 0x33);      /* pressure + temperature, normal mode */
  delay(50);
  return bmpRead();
}

bool OrbiteBoard::bmpRead()
{
  if (_passive && (!bmpId(spiRead8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x00, true)) ||
      (spiRead8(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x03, true) & 0x60) != 0x60)) return false;
  uint8_t d[6];
  spiRead(OrbiteSPI2, SPI_BMP, CS_BMP390, 0x04, d, 6, true);
  uint32_t up = d[0] | ((uint32_t)d[1] << 8) | ((uint32_t)d[2] << 16);
  uint32_t ut = d[3] | ((uint32_t)d[4] << 8) | ((uint32_t)d[5] << 16);
  if (up == 0 || ut == 0) return false;

  double pd1 = (double)ut - _parT1;
  double pd2 = pd1 * _parT2;
  double t = pd2 + (pd1 * pd1) * _parT3;

  pd1 = _parP6 * t;
  pd2 = _parP7 * t * t;
  double pd3 = _parP8 * t * t * t;
  double po1 = _parP5 + pd1 + pd2 + pd3;
  pd1 = _parP2 * t;
  pd2 = _parP3 * t * t;
  pd3 = _parP4 * t * t * t;
  double po2 = (double)up * (_parP1 + pd1 + pd2 + pd3);
  pd1 = (double)up * (double)up;
  pd2 = _parP9 + _parP10 * t;
  pd3 = pd1 * pd2;
  double pd4 = pd3 + pd1 * (double)up * _parP11;
  double p = po1 + po2 + pd4;

  if (p < 1000.0 || p > 120000.0) return false;
  _bmpP = (float)p;
  _bmpT = (float)t;
  _bmpStamp.mark(millis());
  return true;
}

/* ===================================================================== fused altitude */

void OrbiteBoard::baroUpdate()
{
  msTask();
  if (_bmpOK && millis() - _bmpLast >= 20) {        /* 50 Hz */
    _bmpLast = millis();
    bmpRead();
  }
  float a = 0;
  uint8_t n = 0;
  if (_msOK && !isnan(_msP) && !isnan(_msGround)) { a += pressureToAltitude(_msP, _msGround); n++; }
  if (_bmpOK && !isnan(_bmpP) && !isnan(_bmpGround)) { a += pressureToAltitude(_bmpP, _bmpGround); n++; }
  if (n == 0) return;
  _alt = a / n;

  uint32_t now = millis();
  if (_lastAltMs == 0) {
    _lastAltMs = now;
    _lastAlt = _alt;
  } else if (now - _lastAltMs >= 50) {               /* 20 Hz derivative, low-pass */
    float v = (_alt - _lastAlt) * 1000.0f / (now - _lastAltMs);
    _vSpeed = 0.7f * _vSpeed + 0.3f * v;
    _lastAlt = _alt;
    _lastAltMs = now;
  }
}

float OrbiteBoard::pressure()
{
  update();
  float s = 0; uint8_t n = 0;
  if (_msOK && !isnan(_msP)) { s += _msP; n++; }
  if (_bmpOK && !isnan(_bmpP)) { s += _bmpP; n++; }
  return n ? s / n : NAN;
}

float OrbiteBoard::temperature()
{
  update();
  if (_bmpOK && !isnan(_bmpT)) return _bmpT;
  if (_msOK && !isnan(_msT)) return _msT;
  return NAN;
}

float OrbiteBoard::altitude()      { update(); return _alt; }
float OrbiteBoard::verticalSpeed() { update(); return _vSpeed; }

float OrbiteBoard::altitudeSea(float seaLevel_hPa)
{
  float p = pressure();
  return isnan(p) ? NAN : pressureToAltitude(p, seaLevel_hPa * 100.0f);
}

float OrbiteBoard::altitudeMS5611()
{
  return (_msOK && !isnan(_msP) && !isnan(_msGround)) ? pressureToAltitude(_msP, _msGround) : NAN;
}

float OrbiteBoard::altitudeBMP390()
{
  return (_bmpOK && !isnan(_bmpP) && !isnan(_bmpGround)) ? pressureToAltitude(_bmpP, _bmpGround) : NAN;
}

void OrbiteBoard::setGroundLevel()
{
  /* mean over 0.5 s */
  float sm = 0, sb = 0;
  uint16_t nm = 0, nb = 0;
  uint32_t t0 = millis();
  while (millis() - t0 < 500) {
    msTask();
    if (_bmpOK && millis() - _bmpLast >= 20) { _bmpLast = millis(); if (bmpRead()) { sb += _bmpP; nb++; } }
    static float lastMs = NAN;
    if (_msOK && !isnan(_msP) && _msP != lastMs) { sm += _msP; nm++; lastMs = _msP; }
  }
  _msGround = nm ? sm / nm : _msP;
  _bmpGround = nb ? sb / nb : _bmpP;
  _alt = 0;
  _vSpeed = 0;
  _lastAltMs = 0;
}

/* ===================================================================== BMI088 (SPI1) */

bool OrbiteBoard::bmiBegin()
{
  /* accelerometer : a rising edge on CSB switches it to SPI */
  spiRead8(SPI, SPI_BMI, CS_BMI088_ACC, 0x00, true);
  delay(1);
  spiWrite8(SPI, SPI_BMI, CS_BMI088_ACC, 0x7E, 0xB6);          /* soft reset */
  delay(2);
  spiRead8(SPI, SPI_BMI, CS_BMI088_ACC, 0x00, true);          /* back to SPI after reset */
  if (spiRead8(SPI, SPI_BMI, CS_BMI088_ACC, 0x00, true) != 0x1E) return false;
  spiWrite8(SPI, SPI_BMI, CS_BMI088_ACC, 0x7C, 0x00);          /* ACC_PWR_CONF : active */
  delay(5);
  spiWrite8(SPI, SPI_BMI, CS_BMI088_ACC, 0x7D, 0x04);          /* ACC_PWR_CTRL : on */
  delay(50);
  spiWrite8(SPI, SPI_BMI, CS_BMI088_ACC, 0x41, 0x03);          /* ±24 g */
  spiWrite8(SPI, SPI_BMI, CS_BMI088_ACC, 0x40, 0xAB);          /* 800 Hz, normal bandwidth */

  /* gyroscope */
  spiWrite8(SPI, SPI_BMI, CS_BMI088_GYR, 0x14, 0xB6);          /* soft reset */
  delay(30);
  if (spiRead8(SPI, SPI_BMI, CS_BMI088_GYR, 0x00) != 0x0F) return false;
  spiWrite8(SPI, SPI_BMI, CS_BMI088_GYR, 0x0F, 0x00);          /* ±2000 °/s */
  spiWrite8(SPI, SPI_BMI, CS_BMI088_GYR, 0x10, 0x02);          /* 1000 Hz, 116 Hz bandwidth */
  spiWrite8(SPI, SPI_BMI, CS_BMI088_GYR, 0x11, 0x00);          /* normal mode */
  delay(10);
  bmiRead();
  return true;
}

void OrbiteBoard::bmiRead()
{
  if (_passive && (spiRead8(SPI, SPI_BMI, CS_BMI088_ACC, 0x00, true) != 0x1E ||
      spiRead8(SPI, SPI_BMI, CS_BMI088_GYR, 0x00) != 0x0F ||
      !(spiRead8(SPI, SPI_BMI, CS_BMI088_ACC, 0x03, true) & 0x80))) return;
  uint8_t d[6];
  spiRead(SPI, SPI_BMI, CS_BMI088_ACC, 0x12, d, 6, true);
  const float ka = 24.0f * 9.80665f / 32768.0f;
  _ax = (int16_t)(d[0] | (d[1] << 8)) * ka;
  _ay = (int16_t)(d[2] | (d[3] << 8)) * ka;
  _az = (int16_t)(d[4] | (d[5] << 8)) * ka;
  spiRead(SPI, SPI_BMI, CS_BMI088_GYR, 0x02, d, 6);
  const float kg = 2000.0f / 32768.0f;
  _gx = (int16_t)(d[0] | (d[1] << 8)) * kg;
  _gy = (int16_t)(d[2] | (d[3] << 8)) * kg;
  _gz = (int16_t)(d[4] | (d[5] << 8)) * kg;
  _bmiStamp.mark(millis());
}

/* ===================================================================== LSM6DSOX (SPI2) */

bool OrbiteBoard::lsmBegin()
{
  if (spiRead8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x0F) != 0x6C) return false;
  spiWrite8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x12, 0x01);      /* CTRL3_C : software reset */
  delay(10);
  spiWrite8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x12, 0x44);      /* BDU + auto increment */
  spiWrite8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x13, 0x04);      /* CTRL4_C : I2C disabled */
  spiWrite8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x10, 0x64);      /* CTRL1_XL : 416 Hz, ±16 g */
  spiWrite8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x11, 0x6C);      /* CTRL2_G  : 416 Hz, ±2000 °/s */
  delay(10);
  lsmRead();
  return true;
}

void OrbiteBoard::lsmRead()
{
  if (_passive && (spiRead8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x0F) != 0x6C ||
      (spiRead8(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x1E) & 3) != 3)) return;
  uint8_t d[12];
  spiRead(OrbiteSPI2, SPI_LSM, CS_LSM6DSOX, 0x22, d, 12);      /* gyro X..Z then accel X..Z */
  const float kg = 0.070f;                                       /* °/s per LSB at ±2000 */
  const float ka = 0.000488f * 9.80665f;                         /* m/s² per LSB at ±16 g */
  _lgx = (int16_t)(d[0] | (d[1] << 8)) * kg;
  _lgy = (int16_t)(d[2] | (d[3] << 8)) * kg;
  _lgz = (int16_t)(d[4] | (d[5] << 8)) * kg;
  _lax = (int16_t)(d[6] | (d[7] << 8)) * ka;
  _lay = (int16_t)(d[8] | (d[9] << 8)) * ka;
  _laz = (int16_t)(d[10] | (d[11] << 8)) * ka;
  _lsmStamp.mark(millis());
}

void OrbiteBoard::accel(float &x, float &y, float &z) { update(); x = _ax; y = _ay; z = _az; }
void OrbiteBoard::gyro(float &x, float &y, float &z)  { update(); x = _gx; y = _gy; z = _gz; }
void OrbiteBoard::accelLSM(float &x, float &y, float &z) { update(); x = _lax; y = _lay; z = _laz; }
void OrbiteBoard::gyroLSM(float &x, float &y, float &z)  { update(); x = _lgx; y = _lgy; z = _lgz; }

float OrbiteBoard::gForce()
{
  return sqrtf(_ax * _ax + _ay * _ay + _az * _az) / 9.80665f;
}

/* ===================================================================== MMC5983MA (SPI2, BEACON-I) */

/*
 * Offset compensation : the sensor bridge has an offset of up to a few tenths of a gauss. After a SET
 * pulse the output is +B + offset, after a RESET pulse -B + offset, so offset = (SET + RESET) / 2 and
 * B = (SET - RESET) / 2. The offset is measured at start-up, then again every MAG_CAL_EVERY measurements
 * (it drifts with temperature). In between, the sensor stays in the SET state and B = SET - offset.
 */
static const uint8_t MAG_CAL_EVERY = 100;   /* ~1 s at 100 Hz */

static bool magReadField(float b[3])
{
  if (!(spiRead8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x08) & 0x01)) return false;   /* Meas_M_Done */
  uint8_t d[7];
  spiRead(OrbiteSPI2, SPI_MAG, CS_MAG, 0x00, d, 7);
  uint32_t r[3] = {((uint32_t)d[0] << 10) | ((uint32_t)d[1] << 2) | ((d[6] >> 6) & 0x03),
                   ((uint32_t)d[2] << 10) | ((uint32_t)d[3] << 2) | ((d[6] >> 4) & 0x03),
                   ((uint32_t)d[4] << 10) | ((uint32_t)d[5] << 2) | ((d[6] >> 2) & 0x03)};
  for (int i = 0; i < 3; i++) b[i] = ((float)r[i] - 131072.0f) / 16384.0f;
  return true;
}

static void magPulse(uint8_t bit)          /* 0x08 = SET, 0x10 = RESET */
{
  spiWrite8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x09, bit);
  delayMicroseconds(500);
}

static bool magMeasureNow(float b[3])
{
  spiWrite8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x09, 0x01);           /* TM_M */
  uint32_t t0 = millis();
  while (millis() - t0 < 20)
    if (magReadField(b)) return true;
  return false;
}

bool OrbiteBoard::magBegin()
{
  /* R/W bit is bit 7 (1 = read), like the other chips */
  spiWrite8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x0A, 0x80);           /* SW_RST */
  delay(15);
  if (spiRead8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x2F) != 0x30) return false;
  float r[3];
  magPulse(0x08);                                               /* SET */
  if (!magMeasureNow(_magSet)) return false;
  magPulse(0x10);                                               /* RESET */
  if (!magMeasureNow(r)) return false;
  for (int i = 0; i < 3; i++) _magOff[i] = (_magSet[i] + r[i]) / 2;
  magPulse(0x08);                                               /* back to SET */
  _mx = _magSet[0] - _magOff[0];
  _my = _magSet[1] - _magOff[1];
  _mz = _magSet[2] - _magOff[2];
  _magStep = 0;
  _magCount = 0;
  return true;
}

/* steps : 0 start a measurement / 1 read it (SET state)
 *         2 read the SET measurement, RESET, start / 3 read the RESET measurement, SET, update the offset */
void OrbiteBoard::magTask()
{
  if (!_magOK) return;
  if (_magStep == 0) {
    if (++_magCount >= MAG_CAL_EVERY) {
      _magCount = 0;
      magPulse(0x08);                                           /* SET before the first offset measurement */
      _magStep = 2;
    } else {
      _magStep = 1;
    }
    spiWrite8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x09, 0x01);         /* TM_M : start a measurement */
    _magT0 = millis();
    return;
  }
  if (millis() - _magT0 < 10) return;
  if (_passive && spiRead8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x2F) != 0x30) {
    if (_magStep == 3) magPulse(0x08);
    _magStep = 0;
    return;
  }
  float b[3];
  bool ok = magReadField(b);
  if (_magStep == 1) {
    if (ok) {
      _mx = b[0] - _magOff[0];
      _my = b[1] - _magOff[1];
      _mz = b[2] - _magOff[2];
      _magStamp.mark(millis());
    }
    _magStep = 0;
  } else if (_magStep == 2) {
    if (!ok) { _magStep = 0; return; }
    for (int i = 0; i < 3; i++) _magSet[i] = b[i];
    magPulse(0x10);                                             /* RESET */
    spiWrite8(OrbiteSPI2, SPI_MAG, CS_MAG, 0x09, 0x01);
    _magT0 = millis();
    _magStep = 3;
  } else {                                                      /* step 3 : RESET reading */
    magPulse(0x08);                                             /* back to SET in any case */
    _magStep = 0;
    if (!ok) return;
    for (int i = 0; i < 3; i++) _magOff[i] = (_magSet[i] + b[i]) / 2;
    _mx = (_magSet[0] - b[0]) / 2;
    _my = (_magSet[1] - b[1]) / 2;
    _mz = (_magSet[2] - b[2]) / 2;
    _magStamp.mark(millis());
  }
}

void OrbiteBoard::mag(float &x, float &y, float &z) { update(); x = _mx; y = _my; z = _mz; }

float OrbiteBoard::heading()
{
  update();
  float h = atan2f(_my, _mx) * 180.0f / PI;
  return h < 0 ? h + 360.0f : h;
}
