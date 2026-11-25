/*
 * Orbite - LoRa telemetry : SX1262 driver for the Ebyte E22-900M22S (BEACON-I, SPI3)
 * Copyright (c) 2026 Arkyos03 - MIT License
 *
 * No external library : the few SX1262 commands needed are sent directly.
 *  - TCXO powered by DIO3 (1.8 V), falls back to the crystal if it does not start
 *  - RF switch : DIO2 drives TXEN, RXEN is its inverse (74LVC1G04 on BEACON-I)
 *  - DIO1 = TX done / RX done / CRC error interrupt
 */
#include "Orbite.h"
#include "Orbite_Mavlink.h"

static bool sxFault = false;

static const SPISettings SPI_LORA(8000000, MSBFIRST, SPI_MODE0);

/* SX126x opcodes */
#define SX_SET_SLEEP            0x84
#define SX_SET_STANDBY          0x80
#define SX_SET_TX               0x83
#define SX_SET_RX               0x82
#define SX_SET_PACKET_TYPE      0x8A
#define SX_SET_RF_FREQUENCY     0x86
#define SX_SET_TX_PARAMS        0x8E
#define SX_SET_PA_CONFIG        0x95
#define SX_SET_MODULATION       0x8B
#define SX_SET_PACKET_PARAMS    0x8C
#define SX_SET_BUFFER_BASE      0x8F
#define SX_WRITE_BUFFER         0x0E
#define SX_READ_BUFFER          0x1E
#define SX_WRITE_REGISTER       0x0D
#define SX_READ_REGISTER        0x1D
#define SX_SET_DIO_IRQ          0x08
#define SX_GET_IRQ_STATUS       0x12
#define SX_CLEAR_IRQ            0x02
#define SX_SET_DIO2_RF_SWITCH   0x9D
#define SX_SET_DIO3_TCXO        0x97
#define SX_SET_REGULATOR        0x96
#define SX_CALIBRATE            0x89
#define SX_CALIBRATE_IMAGE      0x98
#define SX_GET_RX_BUFFER_STATUS 0x13
#define SX_GET_PACKET_STATUS    0x14
#define SX_GET_DEVICE_ERRORS    0x17
#define SX_CLEAR_DEVICE_ERRORS  0x07
#define SX_GET_STATUS           0xC0

#define IRQ_TX_DONE   0x0001
#define IRQ_RX_DONE   0x0002
#define IRQ_HDR_ERR   0x0020
#define IRQ_CRC_ERR   0x0040
#define IRQ_TIMEOUT   0x0200

static bool waitBusy(uint32_t timeout_ms = 100)
{
  uint32_t t0 = millis();
  while (digitalRead(LORA_BUSY) == HIGH) {
    if (millis() - t0 > timeout_ms) return false;
  }
  return true;
}

static void sxCmd(uint8_t op, const uint8_t *data = nullptr, uint8_t len = 0)
{
  if (sxFault || !waitBusy()) { sxFault = true; return; }
  OrbiteSPI3.beginTransaction(SPI_LORA);
  digitalWrite(CS_LORA, LOW);
  OrbiteSPI3.transfer(op);
  for (uint8_t i = 0; i < len; i++) OrbiteSPI3.transfer(data[i]);
  digitalWrite(CS_LORA, HIGH);
  OrbiteSPI3.endTransaction();
}

static void sxRead(uint8_t op, uint8_t *out, uint8_t len, const uint8_t *pre = nullptr, uint8_t preLen = 0)
{
  if (sxFault || !waitBusy()) { sxFault = true; memset(out, 0, len); return; }
  OrbiteSPI3.beginTransaction(SPI_LORA);
  digitalWrite(CS_LORA, LOW);
  OrbiteSPI3.transfer(op);
  for (uint8_t i = 0; i < preLen; i++) OrbiteSPI3.transfer(pre[i]);
  OrbiteSPI3.transfer(0x00);                 /* status byte */
  for (uint8_t i = 0; i < len; i++) out[i] = OrbiteSPI3.transfer(0x00);
  digitalWrite(CS_LORA, HIGH);
  OrbiteSPI3.endTransaction();
}

static void sxWriteReg(uint16_t addr, uint8_t v)
{
  uint8_t d[3] = {(uint8_t)(addr >> 8), (uint8_t)addr, v};
  sxCmd(SX_WRITE_REGISTER, d, 3);
}

static uint8_t sxReadReg(uint16_t addr)
{
  uint8_t a[2] = {(uint8_t)(addr >> 8), (uint8_t)addr}, v = 0;
  sxRead(SX_READ_REGISTER, &v, 1, a, 2);
  return v;
}

static uint16_t sxIrq()
{
  uint8_t d[2] = {0, 0};
  sxRead(SX_GET_IRQ_STATUS, d, 2);
  return ((uint16_t)d[0] << 8) | d[1];
}

static void sxClearIrq(uint16_t mask = 0xFFFF)
{
  uint8_t d[2] = {(uint8_t)(mask >> 8), (uint8_t)mask};
  sxCmd(SX_CLEAR_IRQ, d, 2);
}

static void sxStandby()
{
  uint8_t d = 0x00;   /* STDBY_RC */
  sxCmd(SX_SET_STANDBY, &d, 1);
}

static uint16_t sxErrors()
{
  uint8_t d[2] = {0, 0};
  sxRead(SX_GET_DEVICE_ERRORS, d, 2);
  return ((uint16_t)d[0] << 8) | d[1];
}

/* ======================================================================= setup */

bool OrbiteBoard::loraBegin(float frequencyMHz, int8_t powerDbm, uint8_t spreadingFactor, uint32_t bandwidthHz, uint16_t preamble)
{
  if (!_begun) begin();
  _loraOK = false;
  _loraTx = _loraRx = _loraListen = false;
  _loraLen = 0;
  sxFault = false;

  digitalWrite(CS_LORA, HIGH);
  pinMode(CS_LORA, OUTPUT);
  pinMode(LORA_BUSY, INPUT);
  pinMode(LORA_DIO1, INPUT);
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(2);
  digitalWrite(LORA_RST, HIGH);
  delay(10);
  if (!waitBusy(200)) { ++_loraErrors; return false; }

  sxStandby();
  /* the chip answers ? (sync word register reads back its default value) */
  uint8_t sw = sxReadReg(0x0740);
  if (sw == 0x00 || sw == 0xFF) { ++_loraErrors; return false; }

  /* TCXO on DIO3 at 1.8 V, 5 ms start-up, then full calibration */
  uint8_t tcxo[4] = {0x02, 0x00, 0x01, 0x40};          /* 1.8 V, 320 x 15.625 us = 5 ms */
  sxCmd(SX_SET_DIO3_TCXO, tcxo, 4);
  /* XOSC_START_ERR est toujours levé à la mise sous tension (avant la commande TCXO) :
   * on l'efface, sinon la calibration semble avoir échoué alors que le TCXO tourne. */
  uint8_t clr0[2] = {0x00, 0x00};
  sxCmd(SX_CLEAR_DEVICE_ERRORS, clr0, 2);
  uint8_t cal = 0x7F;
  sxCmd(SX_CALIBRATE, &cal, 1);
  delay(5);
  waitBusy(100);
  if (sxErrors() & 0x0020) {                           /* XOSC_START_ERR : no TCXO, use the crystal */
    uint8_t clr[2] = {0x00, 0x00};
    sxCmd(SX_CLEAR_DEVICE_ERRORS, clr, 2);
    digitalWrite(LORA_RST, LOW);
    delay(2);
    digitalWrite(LORA_RST, HIGH);
    delay(10);
    waitBusy(200);
    sxStandby();
    sxCmd(SX_CLEAR_DEVICE_ERRORS, clr, 2);           /* idem après ce second reset */
    sxCmd(SX_CALIBRATE, &cal, 1);
    delay(5);
    waitBusy(100);
  }

  uint8_t reg = 0x01;                                  /* DC-DC regulator */
  sxCmd(SX_SET_REGULATOR, &reg, 1);
  uint8_t dio2 = 0x01;                                 /* DIO2 = RF switch (TXEN) */
  sxCmd(SX_SET_DIO2_RF_SWITCH, &dio2, 1);
  uint8_t ptype = 0x01;                                /* LoRa */
  sxCmd(SX_SET_PACKET_TYPE, &ptype, 1);

  /* image calibration for the 863-870 MHz band, then frequency */
  uint8_t img[2] = {0xD7, 0xDB};
  sxCmd(SX_CALIBRATE_IMAGE, img, 2);
  uint32_t frf = (uint32_t)((double)frequencyMHz * 1e6 * 33554432.0 / 32e6);
  uint8_t f[4] = {(uint8_t)(frf >> 24), (uint8_t)(frf >> 16), (uint8_t)(frf >> 8), (uint8_t)frf};
  sxCmd(SX_SET_RF_FREQUENCY, f, 4);

  /* PA : SX1262 high power, up to +22 dBm */
  uint8_t pa[4] = {0x04, 0x07, 0x00, 0x01};
  sxCmd(SX_SET_PA_CONFIG, pa, 4);
  sxWriteReg(0x08E7, 0x38);                            /* OCP 140 mA */
  if (powerDbm > 22) powerDbm = 22;
  if (powerDbm < -9) powerDbm = -9;
  uint8_t txp[2] = {(uint8_t)powerDbm, 0x04};          /* ramp 200 us */
  sxCmd(SX_SET_TX_PARAMS, txp, 2);
  /* datasheet 15.2 : better PA clamping */
  sxWriteReg(0x08D8, sxReadReg(0x08D8) | 0x1E);

  /* modulation : SF, BW, CR 4/5, low data rate optimisation if symbol > 16 ms */
  uint8_t bw;
  switch (bandwidthHz) {
    case 62500:  bw = 0x03; break;
    case 125000: bw = 0x04; break;
    case 500000: bw = 0x06; break;
    default:     bw = 0x05; bandwidthHz = 250000; break;
  }
  if (spreadingFactor < 5) spreadingFactor = 5;
  if (spreadingFactor > 12) spreadingFactor = 12;
  float tsym_ms = (float)(1UL << spreadingFactor) / (bandwidthHz / 1000.0f);
  uint8_t mod[4] = {spreadingFactor, bw, 0x01, (uint8_t)(tsym_ms > 16.0f ? 1 : 0)};
  sxCmd(SX_SET_MODULATION, mod, 4);
  _loraSF = spreadingFactor;
  _loraBW = bandwidthHz;
  _loraPreamble = spreadingFactor <= 6 && preamble < 12 ? 12 : preamble;

  uint8_t base[2] = {0x00, 0x00};
  sxCmd(SX_SET_BUFFER_BASE, base, 2);
  sxWriteReg(0x0740, 0x14);                            /* private sync word 0x12 */
  sxWriteReg(0x0741, 0x24);

  /* DIO1 = TX done | RX done | CRC error | header error | timeout */
  uint16_t mask = IRQ_TX_DONE | IRQ_RX_DONE | IRQ_CRC_ERR | IRQ_HDR_ERR | IRQ_TIMEOUT;
  uint8_t irq[8] = {(uint8_t)(mask >> 8), (uint8_t)mask, (uint8_t)(mask >> 8), (uint8_t)mask, 0, 0, 0, 0};
  sxCmd(SX_SET_DIO_IRQ, irq, 8);
  sxClearIrq();

  _loraOK = !sxFault && sxErrors() == 0 && !sxFault;
  if (!_loraOK) ++_loraErrors;
  _loraTx = false;
  _loraRx = false;
  return _loraOK;
}

bool OrbiteBoard::loraBeginStation()
{
  return loraBegin(869.525f, 22, 6, 250000, 12);
}

static void sxPacketParams(uint8_t len, uint16_t preamble)
{
  /* explicit header, CRC on, standard IQ */
  uint8_t pp[6] = {(uint8_t)(preamble >> 8), (uint8_t)preamble, 0x00, len, 0x01, 0x00};
  sxCmd(SX_SET_PACKET_PARAMS, pp, 6);
}

/* ======================================================================= send / receive */

bool OrbiteBoard::loraSend(const uint8_t *data, uint8_t len)
{
  if (!_loraOK || _loraTx || !data || !len) return false;
  sxStandby();
  sxPacketParams(len, _loraPreamble);
  if (sxFault || !waitBusy()) { sxFault=true; _loraOK=false; ++_loraErrors; return false; }
  OrbiteSPI3.beginTransaction(SPI_LORA);
  digitalWrite(CS_LORA, LOW);
  OrbiteSPI3.transfer(SX_WRITE_BUFFER);
  OrbiteSPI3.transfer(0x00);                           /* offset */
  for (uint8_t i = 0; i < len; i++) OrbiteSPI3.transfer(data[i]);
  digitalWrite(CS_LORA, HIGH);
  OrbiteSPI3.endTransaction();
  sxClearIrq();
  _loraTxTimeout = loraTimeOnAir(len) + 200;
  uint32_t ticks = _loraTxTimeout * 64; // SX1262 RTC: 15.625 us
  uint8_t to[3] = {(uint8_t)(ticks >> 16), (uint8_t)(ticks >> 8), (uint8_t)ticks};
  sxCmd(SX_SET_TX, to, 3);
  if (sxFault) { _loraOK=false; ++_loraErrors; return false; }
  _loraTx = true;
  _loraRx = false;
  _loraTxStart = millis();
  return true;
}

bool OrbiteBoard::loraBusy()
{
  loraTask();
  return _loraTx;
}

void OrbiteBoard::loraListen()
{
  if (!_loraOK) return;
  if (_loraTx) return;              /* listen again once the current packet is sent */
  sxStandby();
  sxPacketParams(255, _loraPreamble);
  sxClearIrq();
  uint8_t to[3] = {0xFF, 0xFF, 0xFF};                  /* continuous RX */
  sxCmd(SX_SET_RX, to, 3);
  if (sxFault) { _loraOK=false; ++_loraErrors; return; }
  _loraRx = true;
  _loraListen = true;
}

void OrbiteBoard::loraTask()
{
  if (!_loraOK) return;
  if (sxFault) { _loraOK=false; _loraTx=false; ++_loraErrors; return; }
  const bool expired = _loraTx && millis() - _loraTxStart >= _loraTxTimeout;
  if (digitalRead(LORA_DIO1) == LOW && !expired) return;
  uint16_t irq = sxIrq();
  if (sxFault) { _loraOK=false; _loraTx=false; ++_loraErrors; return; }
  if (_loraTx && ((irq & (IRQ_TX_DONE | IRQ_TIMEOUT)) || expired)) {
    const bool sent = (irq & IRQ_TX_DONE) && !(irq & IRQ_TIMEOUT);
    sxStandby();
    sxClearIrq();
    _loraTx = false;
    if (sent && !sxFault) ++_loraSent; else ++_loraErrors;
    if (sxFault) { _loraOK=false; return; }
    if (_loraListen) loraListen();
    return;
  }
  if (_loraRx && (irq & IRQ_RX_DONE)) {
    sxClearIrq();
    if (irq & (IRQ_CRC_ERR | IRQ_HDR_ERR)) {
      _loraErrors++;
      return;
    }
    uint8_t st[2] = {0, 0};
    sxRead(SX_GET_RX_BUFFER_STATUS, st, 2);
    uint8_t len = st[0], off = st[1];
    // RX buffer holds the full SX1262 maximum payload (255 bytes).
    uint8_t pre[1] = {off};
    sxRead(SX_READ_BUFFER, _loraBuf, len, pre, 1);
    _loraLen = len;
    uint8_t ps[3] = {0, 0, 0};
    sxRead(SX_GET_PACKET_STATUS, ps, 3);
    _loraRssi = -(float)ps[0] / 2.0f;
    _loraSnr = (float)(int8_t)ps[1] / 4.0f;
    return;
  }
  if (irq) sxClearIrq(irq);
}

bool OrbiteBoard::loraAvailable()
{
  loraTask();
  return _loraLen > 0;
}

uint8_t OrbiteBoard::loraRead(uint8_t *buf, uint8_t maxLen)
{
  loraTask();
  uint8_t n = _loraLen < maxLen ? _loraLen : maxLen;
  memcpy(buf, _loraBuf, n);
  _loraLen = 0;
  return n;
}

uint32_t OrbiteBoard::loraTimeOnAir(uint8_t len)
{
  return orbite::loraAirtimeMs(len, _loraSF, _loraBW, _loraPreamble);
}

/* ======================================================================= telemetry */

static uint16_t crc16(const uint8_t *d, uint8_t n)
{
  uint16_t c = 0xFFFF;
  while (n--) {
    c ^= (uint16_t)*d++ << 8;
    for (uint8_t i = 0; i < 8; i++) c = (c & 0x8000) ? (c << 1) ^ 0x1021 : c << 1;
  }
  return c;
}

void OrbiteBoard::telemetryFill(OrbiteTelemetry &t)
{
  t.magic       = ORBITE_TELEM_MAGIC;
  t.counter     = _telemCounter;
  t.time_ms     = millis();
  t.state       = (uint8_t)_state;
  t.altitude_dm = (int32_t)lroundf(_alt * 10.0f);
  t.maxAlt_dm   = (int32_t)lroundf(_maxAlt * 10.0f);
  t.vspeed_cms  = (int16_t)constrain(lroundf(_vSpeed * 100.0f), -32000, 32000);
  t.accel_cg    = (int16_t)constrain(lroundf(gForce() * 100.0f), -32000, 32000);
  t.lat_e7      = (int32_t)llround(_gpsLat * 1e7);
  t.lon_e7      = (int32_t)llround(_gpsLon * 1e7);
  t.gpsAlt_m    = (int16_t)constrain(lroundf(_gpsAlt), -32000, 32000);
  t.gpsSats     = _gpsSats | (_gpsFix ? 0x80 : 0);
  t.vbat_cv     = (uint16_t)lroundf(batteryVoltage() * 100.0f);
  uint8_t pyro = 0;
  for (uint8_t i = 0; i < 4; i++) {
    if (_fired[i]) pyro |= 1 << i;
    if (pyroArmed() && continuity(i + 1)) pyro |= 0x10 << i;
  }
  t.pyro        = pyro;
  t.flags       = (pyroArmed() ? 0x01 : 0) | (_logOK ? 0x02 : 0) | (pinRemoved() ? 0x04 : 0);
  t.crc         = crc16((const uint8_t *)&t, sizeof(t) - 2);
}

bool OrbiteBoard::telemetrySend()
{
  update();
  if (!_loraOK || _loraTx) return false;
  OrbiteTelemetry t;
  telemetryFill(t);
  if (!loraSend((const uint8_t *)&t, sizeof(t))) return false;
  _telemCounter++;
  return true;
}

bool OrbiteBoard::telemetryReceive(OrbiteTelemetry &t)
{
  if (!loraAvailable()) return false;
  uint8_t buf[sizeof(_loraBuf)];
  uint8_t n = loraRead(buf, sizeof(buf));
  if (n != sizeof(OrbiteTelemetry)) return false;
  memcpy(&t, buf, sizeof(t));
  if (t.magic != ORBITE_TELEM_MAGIC) return false;
  return t.crc == crc16((const uint8_t *)&t, sizeof(t) - 2);
}

void OrbiteBoard::printTelemetryHeader(Print &out)
{
  out.println(F("compteur;temps_ms;etat;altitude_m;alt_max_m;vitesse_ms;g;gps_fix;gps_sats;lat;lon;gps_alt_m;vbat_v;pyro_tir;pyro_cont;arme;sd;rssi_dbm;snr_db"));
}

void OrbiteBoard::printTelemetry(const OrbiteTelemetry &t, Print &out)
{
  static const char *names[] = {"SOL", "MONTEE", "DESCENTE", "POSE"};
  out.print(t.counter); out.print(';');
  out.print(t.time_ms); out.print(';');
  out.print(names[t.state & 3]); out.print(';');
  out.print(t.altitude_dm / 10.0f, 1); out.print(';');
  out.print(t.maxAlt_dm / 10.0f, 1); out.print(';');
  out.print(t.vspeed_cms / 100.0f, 2); out.print(';');
  out.print(t.accel_cg / 100.0f, 2); out.print(';');
  out.print((t.gpsSats & 0x80) ? 1 : 0); out.print(';');
  out.print(t.gpsSats & 0x7F); out.print(';');
  out.print(t.lat_e7 / 1e7, 7); out.print(';');
  out.print(t.lon_e7 / 1e7, 7); out.print(';');
  out.print(t.gpsAlt_m); out.print(';');
  out.print(t.vbat_cv / 100.0f, 2); out.print(';');
  for (uint8_t i = 0; i < 4; i++) out.print((t.pyro >> i) & 1);
  out.print(';');
  for (uint8_t i = 0; i < 4; i++) out.print((t.pyro >> (4 + i)) & 1);
  out.print(';');
  out.print(t.flags & 0x01); out.print(';');
  out.print((t.flags >> 1) & 1); out.print(';');
  out.print(_loraRssi, 1); out.print(';');
  out.println(_loraSnr, 1);
}
