#pragma once
#include "Orbite_Data.h"
namespace orbite {
size_t encodeDiagnostic(const Snapshot &,const RunStatus &,uint8_t *,size_t,bool withStatus);
uint32_t loraAirtimeMs(uint8_t len,uint8_t sf,uint32_t bandwidth,uint16_t preamble);
// At most one packet per 10 times its rounded-up airtime; independent of phase.
class AirtimeBudget {
 public:
  bool ready(uint32_t now)const{return !used_ || uint32_t(now-last_)>=interval_;}
  void record(uint32_t now,uint32_t airtime){last_=now;interval_=airtime>UINT32_MAX/10?UINT32_MAX:airtime*10;used_=true;}
 private:bool used_=false;uint32_t last_=0,interval_=0;
};
}
