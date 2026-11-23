#pragma once
#include <Arduino.h>
#include "Orbite_Data.h"
namespace orbite {
enum class Card { Core, Beacon, Spark, Reactor };
struct BenchResults {
  const char *sd = "NON_TESTABLE";
  uint32_t flash = 0, transmitted = 0, errors = 0;
  bool radio = false;
};
void printDiagnostics(Card, const Snapshot &, Print &, const BenchResults &);
}
