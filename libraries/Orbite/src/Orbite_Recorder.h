#pragma once
#include "Orbite_Data.h"
namespace orbite {
enum class OpenResult { Ok, Exists, Error };
class LogStorage {
 public:
  virtual ~LogStorage() {}
  virtual OpenResult createExclusive(const char *name)=0;
  virtual size_t write(const char *data,size_t len)=0;
  virtual bool sync()=0;
  virtual bool close()=0;
};
const char *csvHeader();
size_t formatCsv(const Snapshot &,const RunStatus &,char *,size_t);
class Recorder {
 public:
  bool begin(LogStorage &,const Snapshot &);
  bool append(const Snapshot &,const RunStatus &);
  bool service(uint32_t now);
  bool close();
  bool ready()const{return ready_;}
  uint32_t errors()const{return errors_;}
  const char *name()const{return name_;}
 private:
  bool add(const char *,size_t);
  bool flush();
  bool fail();
  LogStorage *storage_=nullptr;
  bool ready_=false,opened_=false;
  uint32_t errors_=0,lastSync_=0;
  char name_[13]={},buffer_[4096];
  size_t used_=0;
};
}
