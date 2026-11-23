/*
 * Orbite - flight log on the micro SD card (SDIO 4 bits, STM32SD + FatFs)
 * Copyright (c) 2026 Arkyos03 - MIT License
 */
#include "Orbite.h"
#include <STM32SD.h>

static File logFile;
static bool sdStarted = false;

bool OrbiteBoard::sdCardPresent()
{
  return digitalRead(SD_DETECT) == LOW;   /* Molex 503398 : switch closed to GND = card inserted */
}

static bool sdStart()
{
  if (sdStarted) return true;
  for (uint8_t i = 0; i < 3 && !sdStarted; i++) {
    sdStarted = SD.begin(SD_DETECT);
    if (!sdStarted) delay(50);
  }
  return sdStarted;
}

bool OrbiteBoard::logBegin(const char *prefix, const char *header)
{
  if (!_begun) begin();
  logClose();
  if (!sdCardPresent() || !sdStart()) {
    _logOK = false;
    return false;
  }
  /* first free name : VOL_001.CSV, VOL_002.CSV ... */
  char name[16];
  for (uint16_t i = 1; i < 1000; i++) {
    snprintf(name, sizeof(name), "%.4s_%03u.CSV", prefix, i);
    if (!SD.exists(name)) break;
  }
  logFile = SD.open(name, FILE_WRITE);
  if (!logFile) {
    _logOK = false;
    return false;
  }
  strncpy(_logName, name, sizeof(_logName) - 1);
  _logOK = true;
  _logLastFlush = millis();

  if (header) {
    logFile.println(header);
  } else {
    printFlightHeader(logFile);
  }
  logFile.flush();
  return true;
}

void OrbiteBoard::logStart()
{
  _logFirst = true;
}

void OrbiteBoard::logEnd()
{
  logFile.println();
  /* write to the card at most every 500 ms (or on demand with logFlush) */
  if (millis() - _logLastFlush >= 500) logFlush();
}

void OrbiteBoard::logValue(float v)
{
  if (!_logFirst) logFile.print(_sep);
  _logFirst = false;
  logFile.print(v, 3);
}

void OrbiteBoard::logValue(double v)
{
  if (!_logFirst) logFile.print(_sep);
  _logFirst = false;
  logFile.print(v, 7);
}

void OrbiteBoard::logValue(int v)
{
  if (!_logFirst) logFile.print(_sep);
  _logFirst = false;
  logFile.print(v);
}

void OrbiteBoard::logValue(unsigned int v)
{
  if (!_logFirst) logFile.print(_sep);
  _logFirst = false;
  logFile.print(v);
}

void OrbiteBoard::logValue(const char *v)
{
  if (!_logFirst) logFile.print(_sep);
  _logFirst = false;
  logFile.print(v);
}

void OrbiteBoard::logText(const char *text)
{
  if (!_logOK) return;
  logFile.print(F("# "));
  logFile.println(text);
  logFlush();
}

void OrbiteBoard::logFlush()
{
  if (!_logOK) return;
  logFile.flush();
  _logLastFlush = millis();
}

void OrbiteBoard::logClose()
{
  if (!_logOK) return;
  logFile.flush();
  logFile.close();
  _logOK = false;
}

/* ======================================================================= flight line */

static const char *stateName(OrbiteFlightState s)
{
  switch (s) {
    case ORBITE_GROUND:  return "SOL";
    case ORBITE_ASCENT:  return "MONTEE";
    case ORBITE_DESCENT: return "DESCENTE";
    default:             return "POSE";
  }
}

void OrbiteBoard::printFlightHeader(Print &out)
{
  const char s = _sep;
  out.print(F("temps_ms")); out.print(s); out.print(F("etat")); out.print(s);
  out.print(F("altitude_m")); out.print(s); out.print(F("vitesse_ms")); out.print(s);
  out.print(F("alt_ms5611_m")); out.print(s); out.print(F("alt_bmp390_m")); out.print(s);
  out.print(F("pression_pa")); out.print(s); out.print(F("temperature_c")); out.print(s);
  out.print(F("ax_ms2")); out.print(s); out.print(F("ay_ms2")); out.print(s); out.print(F("az_ms2")); out.print(s);
  out.print(F("gx_dps")); out.print(s); out.print(F("gy_dps")); out.print(s); out.print(F("gz_dps")); out.print(s);
  out.print(F("g")); out.print(s);
  out.print(F("gps_fix")); out.print(s); out.print(F("gps_lat")); out.print(s); out.print(F("gps_lon")); out.print(s);
  out.print(F("gps_alt_m")); out.print(s); out.print(F("gps_sats")); out.print(s);
  out.print(F("vbat_v")); out.print(s); out.print(F("vpyro_v")); out.print(s);
  out.println(F("pyro_1234"));
}

void OrbiteBoard::flightLine(Print &out)
{
  const char s = _sep;
  out.print(millis()); out.print(s);
  out.print(stateName(_state)); out.print(s);
  out.print(_alt, 2); out.print(s);
  out.print(_vSpeed, 2); out.print(s);
  out.print(altitudeMS5611(), 2); out.print(s);
  out.print(altitudeBMP390(), 2); out.print(s);
  out.print(pressure(), 1); out.print(s);
  out.print(temperature(), 2); out.print(s);
  out.print(_ax, 3); out.print(s); out.print(_ay, 3); out.print(s); out.print(_az, 3); out.print(s);
  out.print(_gx, 2); out.print(s); out.print(_gy, 2); out.print(s); out.print(_gz, 2); out.print(s);
  out.print(gForce(), 2); out.print(s);
  out.print(_gpsFix ? 1 : 0); out.print(s);
  out.print(_gpsLat, 7); out.print(s); out.print(_gpsLon, 7); out.print(s);
  out.print(_gpsAlt, 1); out.print(s); out.print(_gpsSats); out.print(s);
  out.print(batteryVoltage(), 2); out.print(s); out.print(pyroVoltage(), 2); out.print(s);
  for (uint8_t i = 0; i < 4; i++) out.print(_fired[i] ? '1' : '0');
  out.println();
}

void OrbiteBoard::logFlight()
{
  update();
  if (!_logOK) return;
  flightLine(logFile);
  if (millis() - _logLastFlush >= 500) logFlush();
}

void OrbiteBoard::printFlight(Print &out)
{
  update();
  flightLine(out);
}

void OrbiteBoard::logTelemetry(const OrbiteTelemetry &t)
{
  if (!_logOK) return;
  printTelemetry(t, logFile);
  if (millis() - _logLastFlush >= 500) logFlush();
}
