#include "Orbite_SD.h"
namespace orbite {
void SDStorage::duration(uint32_t start){uint32_t dt=millis()-start;if(dt>maxIoMs_)maxIoMs_=dt;}
bool SDStorage::mount(){
 if(opened_)return mounted_;
 if(digitalRead(SD_DETECT)!=LOW){mounted_=false;error_=FR_NOT_READY;return false;}
 // A closed session may be followed by insertion of a different card.
 if(attempted_) SD.end();
 attempted_=true;
 uint32_t start=millis();mounted_=SD.begin(SD_DETECT);duration(start);
 error_=mounted_?FR_OK:FR_NOT_READY;return mounted_;
}
OpenResult SDStorage::createExclusive(const char *name){
 if(opened_||!mounted_||digitalRead(SD_DETECT)!=LOW){error_=FR_NOT_READY;return OpenResult::Error;}
 uint32_t start=millis();error_=f_open(&file_,name,FA_WRITE|FA_CREATE_NEW);duration(start);
 if(error_!=FR_OK && error_!=FR_EXIST) mounted_=false;
 if(error_==FR_EXIST)return OpenResult::Exists;
 opened_=error_==FR_OK;return opened_?OpenResult::Ok:OpenResult::Error;
}
size_t SDStorage::write(const char *p,size_t n){
 if(!opened_||digitalRead(SD_DETECT)!=LOW){error_=FR_NOT_READY;return 0;}
 UINT written=0;uint32_t start=millis();error_=f_write(&file_,p,(UINT)n,&written);duration(start);
 if(error_!=FR_OK)return 0;
 if(written!=n)error_=FR_DISK_ERR;
 return written;
}
bool SDStorage::sync(){
 if(!opened_||digitalRead(SD_DETECT)!=LOW){error_=FR_NOT_READY;return false;}
 uint32_t start=millis();error_=f_sync(&file_);duration(start);return error_==FR_OK;
}
bool SDStorage::close(){
 if(!opened_)return true;
 uint32_t start=millis();FRESULT result=f_close(&file_);duration(start);
 opened_=false;
 if(result!=FR_OK){error_=result;mounted_=false;}
 return result==FR_OK;
}
bool SDStorage::verifyReadWrite(char *name,size_t capacity){
 if(!name||capacity<13||opened_||!mount())return false;
 name[0]=0;bool found=false;
 for(unsigned i=1;i<=9999;++i){
  snprintf(name,capacity,"TEST%04u.BIN",i);
  OpenResult result=createExclusive(name);
  if(result==OpenResult::Exists)continue;
  if(result!=OpenResult::Ok)return false;
  found=true;break;
 }
 if(!found){name[0]=0;return false;}
 uint8_t block[256];bool good=true;
 for(unsigned offset=0;offset<4096&&good;offset+=sizeof block){
  for(unsigned j=0;j<sizeof block;++j)block[j]=uint8_t((offset+j)*37u+(offset+j)/256u);
  good=write((const char *)block,sizeof block)==sizeof block;
 }
 if(good)good=sync();
 bool closed=close();if(!good||!closed)return false;
 FIL input={};error_=f_open(&input,name,FA_READ);if(error_!=FR_OK)return false;
 for(unsigned offset=0;offset<4096&&good;offset+=sizeof block){
  UINT count=0;error_=f_read(&input,block,sizeof block,&count);
  good=error_==FR_OK&&count==sizeof block;
  for(unsigned j=0;j<count&&good;++j)good=block[j]==uint8_t((offset+j)*37u+(offset+j)/256u);
 }
 UINT extra=0;if(good){error_=f_read(&input,block,1,&extra);good=error_==FR_OK&&extra==0;}
 FRESULT end=f_close(&input);return good&&end==FR_OK;
}
}
