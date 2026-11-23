/*
 * Orbite - GPS MAX-M10S (BEACON-I) on Serial2 : configuration + NMEA GGA parser
 * Copyright (c) 2026 Arkyos03 - MIT License
 */
#include "Orbite.h"

/* UBX-CFG-VALSET in RAM : dynamic model "airborne < 4 g" and 5 Hz navigation rate */
static void ubxSend(const uint8_t *payload, uint16_t len, uint8_t cls, uint8_t id)
{
  uint8_t ckA = 0, ckB = 0;
  uint8_t hdr[4] = {cls, id, (uint8_t)(len & 0xFF), (uint8_t)(len >> 8)};
  Serial2.write(0xB5);
  Serial2.write(0x62);
  for (uint8_t b : hdr) { Serial2.write(b); ckA += b; ckB += ckA; }
  for (uint16_t i = 0; i < len; i++) { Serial2.write(payload[i]); ckA += payload[i]; ckB += ckA; }
  Serial2.write(ckA);
  Serial2.write(ckB);
}

void OrbiteBoard::gpsBegin()
{
  Serial2.begin(GPS_BAUD);
  if (_passive) return; // Observe the receiver as configured; do not claim an unacknowledged configuration.
  delay(10);
  const uint8_t valset[] = {
    0x00, 0x01, 0x00, 0x00,                    /* version 0, layer RAM, reserved */
    0x21, 0x00, 0x11, 0x20, 0x08,              /* CFG-NAVSPG-DYNMODEL = 8 (airborne < 4 g) */
    0x01, 0x00, 0x21, 0x30, 0xC8, 0x00,        /* CFG-RATE-MEAS = 200 ms (5 Hz) */
  };
  ubxSend(valset, sizeof(valset), 0x06, 0x8A);
}

static double nmeaCoord(const char *s, const char *hemi)
{
  if (!s || !*s) return 0;
  double v = atof(s);
  int deg = (int)(v / 100);
  double d = deg + (v - deg * 100) / 60.0;
  if (hemi && (*hemi == 'S' || *hemi == 'W')) d = -d;
  return d;
}

void OrbiteBoard::gpsLine(char *line)
{
  /* checksum */
  char *star = strchr(line, '*');
  if (!star) return;
  uint8_t ck = 0;
  for (char *p = line + 1; p < star; p++) ck ^= (uint8_t)*p;
  if (strtoul(star + 1, nullptr, 16) != ck) return;
  *star = 0;

  if (strncmp(line + 3, "GGA", 3) != 0) return;     /* $GPGGA, $GNGGA... */
  char *f[16];
  uint8_t n = 0;
  char *p = line;
  while (n < 16) {
    f[n++] = p;
    p = strchr(p, ',');
    if (!p) break;
    *p++ = 0;
  }
  if (n < 10) return;
  /* f[1] time, f[2] lat, f[3] N/S, f[4] lon, f[5] E/W, f[6] fix, f[7] sats, f[8] hdop, f[9] alt */
  if (strlen(f[1]) >= 6) { memcpy(_gpsTime, f[1], 6); _gpsTime[6] = 0; }
  _gpsFix = atoi(f[6]) > 0;
  _gpsSats = atoi(f[7]);
  _gpsHdop = *f[8] ? atof(f[8]) : 99.0f;
  if (_gpsFix) {
    _gpsLat = nmeaCoord(f[2], f[3]);
    _gpsLon = nmeaCoord(f[4], f[5]);
    _gpsAlt = atof(f[9]);
    _gpsMs = millis();
  }
}

void OrbiteBoard::gpsTask()
{
  if (_passive) {
    for (unsigned n = 0; n < 128 && Serial2.available(); ++n) _nmea.feed((char)Serial2.read(), millis());
    const orbite::Gps g = _nmea.snapshot(millis());
    _gpsFix = g.fix; _gpsLat = g.latitude; _gpsLon = g.longitude;
    _gpsAlt = g.altitudeM; _gpsHdop = g.hdop; _gpsSats = g.satellites;
    memcpy(_gpsTime, g.utc, 6); _gpsTime[6] = 0;
    return;
  }
  while (Serial2.available()) {
    char c = Serial2.read();
    if (c == '$') _gpsLen = 0;
    if (c == '\r' || c == '\n') {
      if (_gpsLen > 6) {
        _gpsBuf[_gpsLen] = 0;
        gpsLine(_gpsBuf);
      }
      _gpsLen = 0;
    } else if (_gpsLen < sizeof(_gpsBuf) - 1) {
      _gpsBuf[_gpsLen++] = c;
    }
  }
}

uint32_t OrbiteBoard::gpsAge()
{
  if (_passive) return _nmea.snapshot(millis()).ageMs;
  return _gpsMs ? millis() - _gpsMs : 0xFFFFFFFFUL;
}
