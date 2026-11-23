#pragma once
#include "Orbite_Data.h"
namespace orbite {
class NmeaParser {
 public:
  void feed(char c,uint32_t now);
  Gps snapshot(uint32_t now,uint32_t freshMs=2000) const;
 private:
  void parse(uint32_t now);
  char line_[128]={};
  size_t used_=0;
  bool collecting_=false, overflow_=false, fix_=false;
  uint8_t dimension_=0;
  SampleStamp position_, received_, gsa_;
  Gps value_;
};
}
