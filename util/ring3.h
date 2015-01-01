#pragma once

#include "util/util.h"
#include "devices/mmu32.h"
#include <algorithm>

struct process_t{
  public:
#define process_offset_fpu_simd  0
    unsigned char fpu_simd[1024] ALIGN(0x1000);

#define process_offset_edi       (1024+0)
    uint32_t      edi;

#define process_offset_esi       (1024+4)
    uint32_t      esi;

#define process_offset_ebp       (1024+8)
    uint32_t      ebp;
#define process_offset_esp       (1024+12)
    uint32_t      esp;
#define process_offset_ebx       (1024+16)
    uint32_t      ebx;

#define process_offset_edx       (1024+20)
    uint32_t      edx;
#define process_offset_ecx       (1024+24)
    uint32_t      ecx;
#define process_offset_eax       (1024+28)
    uint32_t      eax;

#define process_offset_eip       (1024+32)
    uint32_t      eip;

#define process_offset_eflags    (1024+36)
    uint32_t      eflags;


#define process_offset_rank      (1024+40)
    uint32_t      rank;

#define process_offset_masterro  (1024+44)
    addr_t        masterro; //one page

#define process_offset_masterrw  (1024+48)
    addr_t        masterrw;

#define process_offset_sharedrw  (1024+52)
    addr_t        sharedrw;

#define process_offset_startip   (1024+56)
    addr_t        startip;

#define process_offset_stackend  (1024+60)
    addr_t        stackend;  //design issue: overflow.

#define process_offset_state     (1024+64)
    uint32_t      state;

#define process_offset_iopl      (1024+68)
    uint32_t      iopl;

#define process_offset_mmu       4096
    dev_pde32_t   mmu;
    /*no tss*/

  public:
    process_t(){
      eip=0;
      eflags=eax=ecx=edx=ebx=esp=ebp=esi=edi=0;
      eax=0x01010101;
      ecx=0x02020202;
      edx=0x03030303;
      ebx=0x04040404;
      esp=0x05050505;
      ebp=0x06060606;
      esi=0x07070707;
      edi=0x08080808;

      masterro=0;
      masterrw=0;
      sharedrw=0;

      state=0;
      iopl=0;

      hoh_assert(offsetof(process_t, fpu_simd) ==0, "XXX");
      hoh_assert(offsetof(process_t, edi) == 1024, "XXX");
      hoh_assert(offsetof(process_t, eip) == 1056, "XXX");
      hoh_assert(offsetof(process_t, mmu) == 4096, "XXX");
    }
};




//
// returns number of 4MB (2^20) pages required
//
static inline size_t elf_numpages(const Elf32_Phdr* prog_header, size_t prog_num){
  enum {PAGE_SIZE=4<<20};
  size_t ret=0;

  addr_t min_start= addr_t(0xffffffff);
  addr_t max_end  = addr_t(0);

  for(int i=0;i<prog_num; i++){
    if(prog_header[i].p_type != PT_LOAD){
      continue;
    }

    addr_t start = prevalign(addr_t(prog_header[i].p_vaddr), PAGE_SIZE);
    addr_t end   = nextalign(addr_t(prog_header[i].p_vaddr + prog_header[i].p_memsz), PAGE_SIZE ); //TODO overflow check

    hoh_assert(start <= end, "XXX");

    min_start = std::min(min_start, start);
    max_end   = std::max(max_end ,  end);

  }

  return (min_start < max_end ) ? (nextalign(max_end,PAGE_SIZE) - prevalign(min_start,PAGE_SIZE))/PAGE_SIZE : 0;

}
