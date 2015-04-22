#pragma once

#include "util/util.h"

static inline void xsyscall(uint32_t* systemcallmmio, uint32_t fnum, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t& ret1, uint32_t& ret2, uint32_t& ret3){

  systemcallmmio[2]=arg1;
  systemcallmmio[3]=arg2;
  systemcallmmio[4]=arg3;
  systemcallmmio[1]=fnum; //write this field at the end.

  hoh_debug("Shell Before making system call");
  asm volatile("int $0x48":::"memory");
  hoh_debug("Shell After making system call");

  hoh_assert(systemcallmmio[1]==0,"XXX");
  ret1=systemcallmmio[2];
  ret2=systemcallmmio[3];
  ret3=systemcallmmio[4];

  hoh_debug("Syscall ret: "<<ret1);

}



