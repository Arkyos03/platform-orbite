/*
 * Orbite - core of the library : start-up, pyro, inputs, buzzer, LED, battery,
 * flight phases, servos, self test.
 * Copyright (c) 2026 Arkyos03 - MIT License
 */
#include "Orbite.h"

OrbiteBoard Orbite;

SPIClass OrbiteSPI2(SPI2_MOSI, SPI2_MISO, SPI2_SCLK);
SPIClass OrbiteSPI3(SPI3_MOSI, SPI3_MISO, SPI3_SCLK);

extern "C" void orbite_enter_bootloader(void);   /* variant_ORBITE_CORE.cpp */

static const uint8_t FIRE_PINS[4]  = {FIRE_1, FIRE_2, FIRE_3, FIRE_4};
static const uint8_t CONT_PINS[4]  = {CONT_1, CONT_2, CONT_3, CONT_4};
static const uint8_t SERVO_PINS[4] = {SERVO_1, SERVO_2, SERVO_3, SERVO_4};

static HardwareTimer *servoTimer = nullptr;   /* TIM4, CH1..CH4 = SERVO_1..4 */

/* ======================================================================= start-up */

void OrbiteBoard::csHigh()
{
  /* every chip select of the 3 SPI buses, on every board of the stack */
  const uint8_t cs[] = {CS_MS5611, CS_BMI088_ACC, CS_BMI088_GYR, CS_BMP390, CS_LSM6DSOX,
                        CS_MAG, CS_LORA, CS_FLASH};
  for (uint8_t p : cs) {
    digitalWrite(p, HIGH);
    pinMode(p, OUTPUT);
  }
}

bool OrbiteBoard::begin()
{
  if (_begun) return true;
  _begun = true;

  /* pyro first : outputs LOW before anything else */
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(FIRE_PINS[i], LOW);
    pinMode(FIRE_PINS[i], OUTPUT);
  }
  csHigh();
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, HIGH);

  pinMode(SW_LAUNCH, INPUT);   /* 10k pull-up + RC filter on SPARK-I */
  pinMode(SW_PIN, INPUT);
  pinMode(SW_AUX, INPUT);
  pinMode(SD_DETECT, INPUT);   /* 10k pull-up on CORE-I */
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  pinMode(LED_RGB, OUTPUT);
  digitalWrite(LED_RGB, LOW);

  analogReadResolution(12);

  if (!_passive) ledShow(0, 0, _brightness);  /* blue : start-up */

  SPI.begin();
  OrbiteSPI2.begin();
  OrbiteSPI3.begin();
  delay(20);

  _msOK  = msBegin();
  _bmpOK = bmpBegin();
  /* MS5611 or MS5607 ? same PROM, different formulas : keep the one that agrees with the BMP */
  if (_msOK && _msModel == orbite::MS56XX_AUTO)
    _msModel = orbite::ms56xxGuess(_msC, _msD1, _msD2, _bmpOK ? _bmpP : NAN);
  _bmiOK = bmiBegin();
  _lsmOK = lsmBegin();
  _magOK = magBegin();
  gpsBegin();

  /* let the barometers settle, then take the ground level */
  uint32_t t0 = millis();
  while (millis() - t0 < 300) update();
  setGroundLevel();
  resetFlight();

  return (_msOK || _bmpOK) && (_bmiOK || _lsmOK);
}

void OrbiteBoard::update()
{
  if (!_begun) return;
  if (_passive) { acquire(); return; }
  pyroUpdate();
  acquire();
  flightUpdate();
  ledUpdate();
}

void OrbiteBoard::acquire()
{
  baroUpdate();
  if (millis() - _imuLast >= 2) {     /* 500 Hz max */
    _imuLast = millis();
    if (_bmiOK) bmiRead();
    if (_lsmOK) lsmRead();
    if (!_bmiOK && _lsmOK) {
      _ax = _lax; _ay = _lay; _az = _laz;
      _gx = _lgx; _gy = _lgy; _gz = _lgz;
    }
  }
  magTask();
  gpsTask();
  loraTask();
  const uint32_t now = millis();
  if (_passive && (!_batteryStamp.seen || _batteryStamp.age(now) >= 20)) {
    _sampleBattery = batteryVoltage();
    _sampleSwitches = (launchSwitch() ? 1 : 0) | (!pinRemoved() ? 2 : 0) | (auxSwitch() ? 4 : 0);
    _batteryStamp.mark(now);
  }
}

void OrbiteBoard::enterBootloader()
{
  orbite_enter_bootloader();
}

/* ======================================================================= pyro */

void OrbiteBoard::fire(uint8_t n, uint32_t duration_ms)
{
  if (n < 1 || n > 4) return;
  uint8_t i = n - 1;
  _fireEnd[i] = millis() + duration_ms;
  _firing[i] = true;
  _fired[i] = true;
  digitalWrite(FIRE_PINS[i], HIGH);
}

void OrbiteBoard::fireStop(uint8_t n)
{
  for (uint8_t i = 0; i < 4; i++) {
    if (n == 0 || n == i + 1) {
      digitalWrite(FIRE_PINS[i], LOW);
      _firing[i] = false;
    }
  }
}

bool OrbiteBoard::firing(uint8_t n)
{
  return (n >= 1 && n <= 4) ? _firing[n - 1] : false;
}

bool OrbiteBoard::fired(uint8_t n)
{
  return (n >= 1 && n <= 4) ? _fired[n - 1] : false;
}

void OrbiteBoard::pyroUpdate()
{
  uint32_t now = millis();
  for (uint8_t i = 0; i < 4; i++) {
    if (_firing[i] && (int32_t)(now - _fireEnd[i]) >= 0) {
      digitalWrite(FIRE_PINS[i], LOW);
      _firing[i] = false;
    }
  }
}

static float adcVolts(uint8_t pin, float ratio)
{
  uint32_t sum = 0;
  for (uint8_t i = 0; i < 4; i++) sum += analogRead(pin);
  return (sum / 4.0f) * 3.3f / 4095.0f * ratio;
}

float OrbiteBoard::pyroVoltage()  { return adcVolts(VPYRO_SENSE, VPYRO_DIVIDER_RATIO); }
bool  OrbiteBoard::pyroArmed(float threshold) { return pyroVoltage() > threshold; }

float OrbiteBoard::continuityVoltage(uint8_t n)
{
  if (n < 1 || n > 4) return 0;
  return adcVolts(CONT_PINS[n - 1], CONT_DIVIDER_RATIO);
}

bool OrbiteBoard::continuity(uint8_t n)
{
  float rail = pyroVoltage();
  if (rail < 3.0f) return false;              /* disarmed : unknown */
  return continuityVoltage(n) > 0.5f * rail;  /* e-match pulls the drain up to the rail */
}

/* ======================================================================= inputs */

bool OrbiteBoard::launchSwitch() { return digitalRead(SW_LAUNCH) == LOW; }
bool OrbiteBoard::pinRemoved()   { return digitalRead(SW_PIN) == HIGH; }
bool OrbiteBoard::auxSwitch()    { return digitalRead(SW_AUX) == LOW; }

/* ======================================================================= battery */

float OrbiteBoard::batteryVoltage() { return adcVolts(VBAT_SENSE, VBAT_DIVIDER_RATIO); }
bool  OrbiteBoard::lowBattery(float threshold) { return batteryVoltage() < threshold; }

/* ======================================================================= buzzer */

void OrbiteBoard::beep(uint32_t frequency, uint32_t duration_ms)
{
  tone(BUZZER, frequency, duration_ms);
}

void OrbiteBoard::beepBlocking(uint32_t frequency, uint32_t duration_ms)
{
  tone(BUZZER, frequency, duration_ms);
  uint32_t t0 = millis();
  while (millis() - t0 < duration_ms) update();
  noTone(BUZZER);
}

void OrbiteBoard::beepCount(uint8_t count, uint32_t frequency)
{
  for (uint8_t i = 0; i < count; i++) {
    beepBlocking(frequency, 120);
    uint32_t t0 = millis();
    while (millis() - t0 < 150) update();
  }
}

void OrbiteBoard::beepAltitude(float meters)
{
  if (meters < 0) meters = maxAltitude();
  long m = lround(meters);
  if (m <= 0) {
    beepBlocking(2000, 1000);
    return;
  }
  char digits[12];
  snprintf(digits, sizeof(digits), "%ld", m);
  for (char *d = digits; *d; d++) {
    uint8_t n = *d - '0';
    if (n == 0) beepBlocking(2000, 600);      /* 0 = one long low beep */
    else beepCount(n, 4000);
    uint32_t t0 = millis();
    while (millis() - t0 < 800) update();
  }
}

void OrbiteBoard::beepContinuity()
{
  for (uint8_t n = 1; n <= 4; n++) {
    if (continuity(n)) beepBlocking(4000, 150);
    else beepBlocking(1500, 150);
    uint32_t t0 = millis();
    while (millis() - t0 < 400) update();
  }
}

/* ======================================================================= RGB LED (SK6805) */

static inline void dwtInit()
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void OrbiteBoard::ledShow(uint8_t r, uint8_t g, uint8_t b)
{
  static bool dwtReady = false;
  if (!dwtReady) {
    dwtInit();
    dwtReady = true;
  }
  GPIO_TypeDef *port = digitalPinToPort(LED_RGB);
  uint32_t mask = digitalPinToBitMask(LED_RGB);
  const uint32_t cpm = SystemCoreClock / 1000000;          /* cycles per µs */
  const uint32_t t0h = cpm * 30 / 100, t1h = cpm * 60 / 100, tbit = cpm * 125 / 100;
  uint32_t grb = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;

  noInterrupts();
  for (int8_t i = 23; i >= 0; i--) {
    uint32_t start = DWT->CYCCNT;
    port->BSRR = mask;
    uint32_t high = (grb & (1UL << i)) ? t1h : t0h;
    while (DWT->CYCCNT - start < high) {}
    port->BSRR = mask << 16;
    while (DWT->CYCCNT - start < tbit) {}
  }
  interrupts();
  delayMicroseconds(100);                                   /* reset (> 80 µs) */
}

void OrbiteBoard::rgb(uint8_t r, uint8_t g, uint8_t b)
{
  _statusLed = false;
  ledShow(r, g, b);
}

void OrbiteBoard::rgb(uint32_t c)
{
  rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

void OrbiteBoard::statusLed(bool on)        { _statusLed = on; if (!on) ledShow(0, 0, 0); }
void OrbiteBoard::statusBrightness(uint8_t level) { _brightness = level; }

void OrbiteBoard::ledUpdate()
{
  if (!_statusLed || millis() - _ledLast < 50) return;
  _ledLast = millis();
  uint32_t t = millis();
  uint8_t br = _brightness;
  /* breathing 0..1 over 2 s, blinking at 4 Hz */
  float breath = 0.5f - 0.5f * cosf((t % 2000) * 2.0f * PI / 2000.0f);
  bool blink = (t / 125) & 1;
  uint8_t r = 0, g = 0, b = 0;
  switch (_state) {
    case ORBITE_GROUND:
      if (lowBattery()) { r = blink ? br : 0; g = blink ? br / 3 : 0; }      /* orange blink */
      else if (pyroArmed()) { r = br * breath; }                             /* red breathing : ARMED */
      else { g = br * breath; }                                              /* green breathing : ready */
      break;
    case ORBITE_ASCENT:  b = blink ? br : 0; break;                          /* blue blink */
    case ORBITE_DESCENT: r = blink ? br : 0; b = blink ? br : 0; break;      /* purple blink */
    case ORBITE_LANDED:  r = g = b = ((t % 1000) < 300) ? br : 0; break;     /* white flash */
  }
  ledShow(r, g, b);
}

/* ======================================================================= flight */

void OrbiteBoard::flightUpdate()
{
  uint32_t now = millis();
  float g = gForce();
  if (_state != ORBITE_GROUND && g > _maxG) _maxG = g;

  switch (_state) {
    case ORBITE_GROUND:
      if ((_alt > _liftoffAlt) || (_liftoffG > 0 && g > _liftoffG)) {
        _state = ORBITE_ASCENT;
        _liftoffMs = now;
        _maxAlt = _alt;
        _maxG = g;
      }
      break;
    case ORBITE_ASCENT:
      if (_alt > _maxAlt) _maxAlt = _alt;
      if (now - _liftoffMs > _apogeeMinTime &&
          ((_maxAlt - _alt) > _apogeeDrop ||
           (_apogeeTimeout > 0 && now - _liftoffMs > _apogeeTimeout))) {
        _state = ORBITE_DESCENT;
        _apogeeMs = now;
        _stillAlt = _alt;
        _stillMs = now;
      }
      break;
    case ORBITE_DESCENT:
      if (fabsf(_alt - _stillAlt) > 2.0f) {
        _stillAlt = _alt;
        _stillMs = now;
      } else if (now - _stillMs > _landingTime && now - _apogeeMs > 5000) {
        _state = ORBITE_LANDED;
      }
      break;
    case ORBITE_LANDED:
      break;
  }
}

bool OrbiteBoard::liftoff() { update(); return _state >= ORBITE_ASCENT; }
bool OrbiteBoard::apogee()  { update(); return _state >= ORBITE_DESCENT; }
bool OrbiteBoard::landed()  { update(); return _state == ORBITE_LANDED; }

uint32_t OrbiteBoard::flightTime()
{
  return (_state == ORBITE_GROUND) ? 0 : millis() - _liftoffMs;
}

float OrbiteBoard::maxAltitude() { return _maxAlt; }

void OrbiteBoard::resetFlight()
{
  _state = ORBITE_GROUND;
  _liftoffMs = _apogeeMs = 0;
  _maxAlt = 0;
  _maxG = 0;
}

/* ======================================================================= servos */

void OrbiteBoard::servoMicroseconds(uint8_t n, uint16_t us)
{
  if (n < 1 || n > 4) return;
  if (us < 500) us = 500;
  if (us > 2500) us = 2500;
  if (!servoTimer) {
    servoTimer = new HardwareTimer(TIM4);
    servoTimer->setOverflow(20000, MICROSEC_FORMAT);      /* 50 Hz */
  }
  uint8_t i = n - 1;
  if (!_servoOn[i]) {
    servoTimer->setMode(n, TIMER_OUTPUT_COMPARE_PWM1, SERVO_PINS[i]);
    _servoOn[i] = true;
  }
  servoTimer->setCaptureCompare(n, us, MICROSEC_COMPARE_FORMAT);
  servoTimer->resume();
}

void OrbiteBoard::servoWrite(uint8_t n, int angle)
{
  angle = constrain(angle, 0, 180);
  servoMicroseconds(n, 500 + (uint32_t)angle * 2000 / 180);
}

void OrbiteBoard::servoAngles(uint8_t n, int closedAngle, int openAngle)
{
  if (n < 1 || n > 4) return;
  _servoClosed[n - 1] = closedAngle;
  _servoOpen[n - 1] = openAngle;
}

void OrbiteBoard::servoOpen(uint8_t n)  { if (n >= 1 && n <= 4) servoWrite(n, _servoOpen[n - 1]); }
void OrbiteBoard::servoClose(uint8_t n) { if (n >= 1 && n <= 4) servoWrite(n, _servoClosed[n - 1]); }

void OrbiteBoard::servoDetach(uint8_t n)
{
  if (n < 1 || n > 4 || !servoTimer || !_servoOn[n - 1]) return;
  servoTimer->setCaptureCompare(n, 0, MICROSEC_COMPARE_FORMAT);   /* no pulse */
}

/* ======================================================================= self test */

OrbiteError OrbiteBoard::selfTest(bool needSdCard, bool needBattery, uint8_t pyroMask)
{
  if (!_begun) begin();
  OrbiteError err = ORBITE_OK;
  float vbat = batteryVoltage();
  float vpyro = pyroVoltage();

  Serial.println(F("=== ORBITE-I : auto-test ==="));
  Serial.print(F("MS5611/07 : ")); Serial.println(_msOK ? ms56xxName() : "absent");
  Serial.print(F("BMP390/388: ")); Serial.println(_bmpOK ? F("OK") : F("absent"));
  Serial.print(F("BMI088    : ")); Serial.println(_bmiOK ? F("OK") : F("absent"));
  Serial.print(F("LSM6DSOX  : ")); Serial.println(_lsmOK ? F("OK") : F("absent"));
  Serial.print(F("MMC5983MA : ")); Serial.println(_magOK ? F("OK") : F("absent"));
  Serial.print(F("Carte SD  : ")); Serial.println(sdCardPresent() ? F("présente") : F("absente"));
  Serial.print(F("Batterie  : ")); Serial.print(vbat, 2); Serial.println(F(" V"));
  Serial.print(F("Rail pyro : ")); Serial.print(vpyro, 2); Serial.println(vpyro > 3.0f ? F(" V (ARMÉ)") : F(" V (désarmé)"));
  for (uint8_t n = 1; n <= 4; n++) {
    Serial.print(F("Pyro ")); Serial.print(n); Serial.print(F("    : "));
    Serial.println(vpyro < 3.0f ? F("? (désarmé)") : (continuity(n) ? F("continuité OK") : F("ouvert")));
  }

  if (!_msOK && !_bmpOK) err = ORBITE_ERR_BARO;
  else if (!_bmiOK && !_lsmOK) err = ORBITE_ERR_IMU;
  else if (needSdCard && !sdCardPresent()) err = ORBITE_ERR_SD;
  else if (needBattery && vbat < 10.5f) err = ORBITE_ERR_BATTERY;
  else if (pyroMask) {
    for (uint8_t n = 1; n <= 4; n++)
      if ((pyroMask & (1 << (n - 1))) && vpyro > 3.0f && !continuity(n)) err = ORBITE_ERR_PYRO;
  }

  if (err == ORBITE_OK) {
    Serial.println(F("=> tout est OK"));
    beepCount(3, 4000);
  } else {
    Serial.print(F("=> ERREUR ")); Serial.println((int)err);
    beepBlocking(1500, 800);
    uint32_t t0 = millis();
    while (millis() - t0 < 400) update();
    beepCount((uint8_t)err, 4000);
  }
  return err;
}
