#include "Orbite_Nmea.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
namespace orbite {
static int hex(char c) {
 if(c>='0'&&c<='9')return c-'0'; if(c>='A'&&c<='F')return c-'A'+10;
 if(c>='a'&&c<='f')return c-'a'+10; return -1;
}
static bool number(const char *s,double &v) {
 if(!s || !*s || isspace(static_cast<unsigned char>(*s)))return false;
 char *end=nullptr; v=strtod(s,&end); return end!=s && *end==0 && isfinite(v);
}
static bool integer(const char *s,unsigned maxValue,unsigned &v) {
 if(!s||!*s)return false; v=0;
 for(;*s;++s) { if(*s<'0'||*s>'9')return false; v=v*10+(*s-'0'); if(v>maxValue)return false; }
 return true;
}
static bool coord(const char *s,const char *hem,bool latitude,double &v) {
 size_t digits=latitude?4:5;
 if(strlen(s)<digits || (s[digits] && s[digits]!='.'))return false;
 for(size_t i=0;i<digits;++i)if(!isdigit(static_cast<unsigned char>(s[i])))return false;
 if(strlen(hem)!=1 || (latitude ? (*hem!='N'&&*hem!='S') : (*hem!='E'&&*hem!='W')))return false;
 double raw; if(!number(s,raw)||raw<0)return false;
 unsigned deg=static_cast<unsigned>(raw/100); double minutes=raw-deg*100;
 if(minutes>=60 || deg>(latitude?90u:180u) || (deg==(latitude?90u:180u)&&minutes>0))return false;
 v=deg+minutes/60.; if(*hem=='S'||*hem=='W')v=-v; return true;
}
static bool utc(const char *s) {
 if(strlen(s)<6 || strlen(s)>10 || (s[6] && s[6]!='.'))return false;
 for(int i=0;i<6;++i)if(!isdigit(static_cast<unsigned char>(s[i])))return false;
 for(size_t i=7;i<strlen(s);++i)if(!isdigit(static_cast<unsigned char>(s[i])))return false;
 return (s[0]-'0')*10+s[1]-'0'<24 && (s[2]-'0')*10+s[3]-'0'<60 && (s[4]-'0')*10+s[5]-'0'<=60;
}
void NmeaParser::feed(char c,uint32_t now) {
 if(c=='$') { used_=0; collecting_=true; overflow_=false; line_[used_++]=c; return; }
 if(!collecting_)return;
 if(c=='\r'||c=='\n') {
   collecting_=false;
   if(!overflow_) { line_[used_]=0; parse(now); }
   return;
 }
 if(used_+1<sizeof line_)line_[used_++]=c; else overflow_=true;
}
void NmeaParser::parse(uint32_t now) {
 char *star=strchr(line_,'*');
 if(!star || strlen(star)!=3 || hex(star[1])<0 || hex(star[2])<0)return;
 uint8_t ck=0; for(char *p=line_+1;p<star;++p)ck^=static_cast<uint8_t>(*p);
 if(ck!=((hex(star[1])<<4)|hex(star[2])))return;
 *star=0;
 char *f[24]; unsigned n=0; char *p=line_;
 do { f[n++]=p; p=strchr(p,','); if(p)*p++=0; } while(p && n<24);
 if(strlen(f[0])!=6 || f[0][0]!='$')return;
 if(strcmp(f[0]+3,"GSA")==0) {
   unsigned d;
   if(n<18 || (strcmp(f[1],"A")&&strcmp(f[1],"M")) || !integer(f[2],3,d)||d<1)return;
   dimension_=d; gsa_.mark(now); received_.mark(now); return;
 }
 if(strcmp(f[0]+3,"GGA") || n<15)return;
 unsigned q,sats;
 if(!utc(f[1]) || !integer(f[6],8,q) || !integer(f[7],99,sats))return;
 if(q==0 || q>5) { // no fix, estimated/dead-reckoned/manual position not a measured fix
   fix_=false; value_.quality=q; value_.satellites=sats; received_.mark(now); return;
 }
 double lat,lon,alt,hdop;
 if(!coord(f[2],f[3],true,lat)||!coord(f[4],f[5],false,lon)||
    !number(f[9],alt)||!number(f[8],hdop)||hdop<0||hdop>999||strcmp(f[10],"M"))return;
 if(alt < -20000 || alt > 100000)return;
 value_.latitude=lat; value_.longitude=lon; value_.altitudeM=alt; value_.hdop=hdop;
 value_.satellites=sats; value_.quality=q; value_.hasPosition=true;
 strncpy(value_.utc,f[1],sizeof value_.utc-1); value_.utc[sizeof value_.utc-1]=0;
 position_.mark(now); received_.mark(now); fix_=true;
}
Gps NmeaParser::snapshot(uint32_t now,uint32_t freshMs) const {
 Gps out=value_; out.ageMs=position_.age(now); out.rxAgeMs=received_.age(now);
 out.gsaAgeMs=gsa_.age(now); out.sequence=position_.sequence;
 out.fix=fix_ && position_.fresh(now,freshMs);
 if(gsa_.fresh(now,freshMs) && dimension_==1)out.fix=false;
 out.fix3d=out.fix && gsa_.fresh(now,freshMs) && dimension_==3;
 out.fixType=out.fix3d?3:(out.fix && gsa_.fresh(now,freshMs)&&dimension_==2?2:(received_.seen?1:0));
 return out;
}
}
