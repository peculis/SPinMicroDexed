#!/usr/bin/env python3
"""Run the actual configuration serializers against an in-memory File adapter."""
from pathlib import Path
import subprocess
import tempfile

source = (Path(__file__).resolve().parents[1] / 'dexed_sd.cpp').read_text()
start = source.index('bool get_sd_data(')
end = source.index('void strip_extension(', start)
functions = source[start:end]
harness = r'''
#include <cstdint>
#include <cstddef>
#include <vector>
#include <memory>
#include <cstring>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <cstdio>
#define FILENAME_LEN 22
#define FILE_WRITE 1
struct State { std::vector<uint8_t> bytes; size_t pos=0; bool fail=false; };
struct File {
  std::shared_ptr<State> state=std::make_shared<State>();
  bool valid=true;
  operator bool() { return valid; }
  void close() {}
  size_t size() { return state->bytes.size(); }
  bool seek(size_t p) { if(p>size()) return false; state->pos=p; return true; }
  int read() { return state->pos<size()?state->bytes[state->pos++]:-1; }
  int read(uint8_t* target, size_t len) {
    size_t n=0; while(n<len && state->pos<size()) target[n++]=read(); return n;
  }
  size_t write(uint8_t value) {
    if(state->fail) return 0;
    if(state->pos==size()) state->bytes.push_back(value);
    else state->bytes[state->pos]=value;
    ++state->pos; return 1;
  }
  size_t write(uint8_t* data, size_t len) {
    size_t n=0; while(n<len && write(data[n])) ++n; return n;
  }
  void flush() {}
};
struct FakeSD {
  std::map<std::string,File> files;
  bool failWrite=false, failBackup=false, failInstall=false;
  bool exists(const char* p) { return files.count(p); }
  bool remove(const char* p) { return files.erase(p); }
  File open(const char* p, int) {
    File f; f.state->fail=failWrite; files[p]=f; return f;
  }
  bool rename(const char* a, const char* b) {
    if ((failBackup && std::string(b).find(".bak")!=std::string::npos) ||
        (failInstall && std::string(a).find(".tmp")!=std::string::npos) ||
        !files.count(a) || files.count(b)) return false;
    files[b]=files[a]; files.erase(a); return true;
  }
} SD;
uint8_t calc_checksum(uint8_t*, uint16_t);
bool install_staged_sd_file(const char*, const char*, const char*);
'''
checks = r'''
int main() {
  uint8_t data[]={0,127,128,255,42};
  File good;
  assert(write_sd_data(good,0x42,data,sizeof(data)));
  assert(good.size()==sizeof(data)+5);
  uint8_t output[sizeof(data)]={};
  assert(get_sd_data(good,0x42,output,sizeof(output)));
  assert(memcmp(data,output,sizeof(data))==0);
  for(size_t i=0; i<good.size(); ++i) {
    File bad; bad.state->bytes=good.state->bytes; bad.state->bytes[i]^=1;
    memset(output,77,sizeof(output));
    assert(!get_sd_data(bad,0x42,output,sizeof(output)));
    for(auto value:output) assert(value==77);
  }
  File shortfile; shortfile.state->bytes=good.state->bytes;
  shortfile.state->bytes.pop_back();
  assert(!get_sd_data(shortfile,0x42,output,sizeof(output)));
  File longfile; longfile.state->bytes=good.state->bytes;
  longfile.state->bytes.push_back(0);
  assert(!get_sd_data(longfile,0x42,output,sizeof(output)));
  assert(!get_sd_data(good,0x44,output,sizeof(output)));
  File failed; failed.state->fail=true;
  assert(!write_sd_data(failed,0x42,data,sizeof(data)));
  assert(!write_sd_data(longfile,0x42,data,sizeof(data)));
  const char* slot="/VCFG/VCFG0.syx";
  assert(replace_sd_configuration(slot,0x42,data,sizeof(data)));
  auto original=SD.files[slot].state->bytes;
  data[4]=43;
  SD.failWrite=true;
  assert(!replace_sd_configuration(slot,0x42,data,sizeof(data)));
  assert(SD.files[slot].state->bytes==original);
  SD.failWrite=false; SD.failBackup=true;
  assert(!replace_sd_configuration(slot,0x42,data,sizeof(data)));
  assert(SD.files[slot].state->bytes==original);
  SD.failBackup=false; SD.failInstall=true;
  assert(!replace_sd_configuration(slot,0x42,data,sizeof(data)));
  assert(SD.files[slot].state->bytes==original);
  SD.failInstall=false;
  assert(replace_sd_configuration(slot,0x42,data,sizeof(data)));
  assert(SD.files[slot].state->bytes!=original);
  assert(!SD.exists("/VCFG/VCFG0.syx.bak"));
  SD.files["/VCFG/VCFG0.syx.bak"]=File();
  assert(!replace_sd_configuration(slot,0x42,data,sizeof(data)));
  uint8_t bank[4104]={};
  bank[0]=0xf0; bank[1]=0x43; bank[3]=9; bank[4]=32; bank[4103]=0xf7;
  assert(valid_dx7_bank(bank));
  assert(replace_sd_bank_file("/0/test.syx",bank));
  auto bank_original=SD.files["/0/test.syx"].state->bytes;
  bank[6]=5; bank[4102]=calc_checksum(bank+6,4096);
  SD.failWrite=true;
  assert(!replace_sd_bank_file("/0/test.syx",bank));
  assert(SD.files["/0/test.syx"].state->bytes==bank_original);
  SD.failWrite=false; SD.failInstall=true;
  assert(!replace_sd_bank_file("/0/test.syx",bank));
  assert(SD.files["/0/test.syx"].state->bytes==bank_original);
  SD.failInstall=false;
  assert(replace_sd_bank_file("/0/test.syx",bank));
  bank[6]^=1; assert(!valid_dx7_bank(bank));
  assert(!replace_sd_bank_file("/0/test.syx",bank));
  bank[6]^=1; bank[7]=128; assert(!valid_dx7_bank(bank));
  std::cout << "Configuration storage tests passed: exact round trip including last byte, "
            << "corruption rejection without mutation, size/type rejection, write failures.\n";
}
'''
with tempfile.TemporaryDirectory(prefix='spin-config-test-') as folder:
    cpp=Path(folder)/'test.cpp'; exe=Path(folder)/'test'
    cpp.write_text(harness+functions+checks)
    subprocess.run(['c++','-std=c++11','-Wall','-Wextra',str(cpp),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
