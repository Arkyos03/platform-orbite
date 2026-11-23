#pragma once
#include "Orbite_Recorder.h"
#include <STM32SD.h>
namespace orbite {
// Owns the SD volume in the passive examples. Do not mix with Orbite.logBegin().
class SDStorage:public LogStorage {
 public:
  bool mount();
  OpenResult createExclusive(const char *) override;
  size_t write(const char *,size_t) override;
  bool sync() override;
  bool close() override;
  bool verifyReadWrite(char *name,size_t capacity); // 4096-byte pattern, retained file
  uint32_t maxIoMs() const {return maxIoMs_;}
  FRESULT error() const {return error_;}
 private:
  void duration(uint32_t start);
  FIL file_={};
  bool attempted_=false;
  bool mounted_=false,opened_=false;
  uint32_t maxIoMs_=0;
  FRESULT error_=FR_OK;
};
}
