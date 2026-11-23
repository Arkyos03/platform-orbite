#include "Orbite_Diagnostics.h"
namespace orbite {
void printDiagnostics(Card card, const Snapshot &s, Print &out, const BenchResults &bench) {
  auto report = [&](const char *label, const char *result) {
    out.print(s.timeMs); out.print(';'); out.print(label); out.print(';'); out.println(result);
  };
  auto sensor = [&](const char *label, Sensor bit, uint32_t age) {
    if (!(s.presentMask & bit)) report(label, "ABSENT");
    else if (s.validMask & bit) report(label, "PASS");
    else report(label, age == UINT32_MAX ? "ATTENTE" : "FAIL");
  };
  if (card == Card::Core) {
    sensor("MS5611", MS5611, s.ms.ageMs);
    sensor("BMP390", BMP390, s.bmp.ageMs);
    sensor("BMI088", BMI088, s.bmi.ageMs);
    sensor("LSM6DSOX", LSM6DSOX, s.lsm.ageMs);
    report("FLASH_JEDEC", bench.flash == 0xef4018 ? "PASS" : "FAIL");
    report("SD", bench.sd);
  }
  if (card == Card::Beacon) {
    sensor("MMC5983MA", MAG, s.mag.ageMs);
    report("GPS_NMEA", s.gps.rxAgeMs <= 2000 ? "PASS" : "ATTENTE");
    report("GPS_3D", s.gps.fix3d ? "PASS" : "ATTENTE");
    report("LORA_INIT", bench.radio ? "PASS" : "FAIL");
    report("LORA_TX", bench.errors ? "FAIL" : bench.transmitted ? "PASS" : "ATTENTE");
    report("RECEPTION_STATION", "NON_TESTABLE");
    report("LED_VISIBLE", "NON_TESTABLE");
  }
  if (card == Card::Spark) {
    out.print(s.timeMs); out.print(";ENTREES_FERMEES;"); out.println(s.switches);
    report("BUZZER_AUDIBLE", "NON_TESTABLE");
  }
  if (card == Card::Core || card == Card::Reactor) {
    out.print(s.timeMs); out.print(";BATTERIE_V;"); out.println(s.batteryV, 3);
  }
  if (card == Card::Reactor) {
    report("RAILS_3V3_5V_6V", "NON_TESTABLE");
    report("SERVOS", "NON_TESTABLE");
  }
}
}
