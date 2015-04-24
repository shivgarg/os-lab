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



struct usertrapframe_t{
  uint32_t rank;
  addr_t   masterro;
  addr_t   masterrw;
  addr_t   sharedrw;
  uint32_t num;
  uint32_t errorcode;
  addr_t   old_esp;
  addr_t   old_eip;
  uint32_t cr2;
};

extern "C" void handle_user_fault(usertrapframe_t& x) STACK_FORCE_ALIGN;