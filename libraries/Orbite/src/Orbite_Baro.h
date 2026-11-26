/* Compensation of the MS56xx barometers (MS5611-01BA03, MS5607-02BA03). No Arduino dependency.
 * Both parts share the package, pinout, SPI commands and PROM CRC : only the exponents of the
 * compensation differ (datasheets, "Pressure and temperature calculation" and "Second order").
 * MIT, ORBITE-I contributors. */
#pragma once
#include <stdint.h>

namespace orbite {

enum Ms56xx : uint8_t { MS56XX_AUTO = 0, MS56XX_5611 = 1, MS56XX_5607 = 2 };

/* D1, D2 : raw 24-bit conversions ; C : PROM words (C[1]..C[6] used).
 * P in Pa (= 0.01 mbar), T in 0.01 degC. false if the result is out of the sensor range. */
inline bool ms56xxCompensate(Ms56xx model, const uint16_t C[8], uint32_t D1, uint32_t D2, int32_t &P, int32_t &T)
{
  const bool m07 = model == MS56XX_5607;
  int64_t dT = (int64_t)D2 - ((int64_t)C[5] << 8);
  int64_t temp = 2000 + ((dT * C[6]) >> 23);
  int64_t off, sens;
  if (m07) {
    off = ((int64_t)C[2] << 17) + (((int64_t)C[4] * dT) >> 6);
    sens = ((int64_t)C[1] << 16) + (((int64_t)C[3] * dT) >> 7);
  } else {
    off = ((int64_t)C[2] << 16) + (((int64_t)C[4] * dT) >> 7);
    sens = ((int64_t)C[1] << 15) + (((int64_t)C[3] * dT) >> 8);
  }
  if (temp < 2000) { /* second order */
    int64_t t2 = (dT * dT) >> 31;
    int64_t d = (temp - 2000) * (temp - 2000);
    int64_t off2 = m07 ? 61 * d / 16 : 5 * d / 2;
    int64_t sens2 = m07 ? 2 * d : 5 * d / 4;
    if (temp < -1500) {
      int64_t e = (temp + 1500) * (temp + 1500);
      off2 += m07 ? 15 * e : 7 * e;
      sens2 += m07 ? 8 * e : 11 * e / 2;
    }
    temp -= t2;
    off -= off2;
    sens -= sens2;
  }
  int64_t p = ((((int64_t)D1 * sens) >> 21) - off) >> 15;
  P = (int32_t)p;
  T = (int32_t)temp;
  return D1 != 0 && D1 != 0xFFFFFF && D2 != 0 && D2 != 0xFFFFFF && p >= 1000 && p <= 120000 && temp >= -6000 &&
         temp <= 10000;
}

/* Which part is soldered ? The PROM does not tell. Compute the pressure with both formulas and keep the
 * one closest to a reference (the other barometer) : the two results differ by tens of kPa. */
inline Ms56xx ms56xxGuess(const uint16_t C[8], uint32_t D1, uint32_t D2, float refPa)
{
  int32_t p11, p07, t;
  bool ok11 = ms56xxCompensate(MS56XX_5611, C, D1, D2, p11, t);
  bool ok07 = ms56xxCompensate(MS56XX_5607, C, D1, D2, p07, t);
  if (ok07 && !ok11) return MS56XX_5607;
  if (!ok07 || !(refPa > 0)) return MS56XX_5611;
  float e11 = p11 - refPa, e07 = p07 - refPa;
  return (e07 * e07 < e11 * e11) ? MS56XX_5607 : MS56XX_5611;
}

} // namespace orbite
