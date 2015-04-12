#pragma once

#include "util/elf.h"
#include "util/ring3.h"
#include "util/bitpool.h"
#include "devices/mmu32.h"
#include "devices/gdt32.h"
#include <stddef.h>
#include <algorithm>

#define pgsize 4096*1024

static inline void elf_load(addr_t from, size_t fromsize, process_t& proc, bitpool_t& pool4M){
  enum {PAGE_SIZE=4<<20};

  hoh_assert(elf_canread_prog_header(from,fromsize), "XXX");
  const Elf32_Ehdr& header      = *(Elf32_Ehdr*)(from);
  uint32_t prog_offset          = header.e_phoff;
  const Elf32_Phdr* prog_header = (Elf32_Phdr*)(from+prog_offset);
  uint32_t    prog_num          = header.e_phnum;

  elf_dump(from, fromsize);

  size_t numpages = elf_numpages(prog_header,prog_num);
  hoh_assert(numpages == 1, "XXX");

  addr_t to = alloc(pool4M); 
// ELF LOADED
  /*for(int i=0;i<prog_num;i++)
  {
	if(prog_header[i].p_type!=PT_LOAD){
		continue;
	}
	memcpy(to+prog_header[i].p_vaddr,(void *)(prog_header[i].p_offset+from),prog_header[i].p_memsz);
	memset(to+prog_header[i].p_vaddr+prog_header[i].p_filesz,0,prog_header[i].p_memsz-prog_header[i].p_filesz);

	
  }
	
 //PROC INITIALISATION
  proc.eip=(uint)header.e_entry;
  proc.eflags=0x3200;
  proc.rank=0;
  proc.masterro=0;
  proc.masterrw=alloc(pool4M);
  proc.sharedrw=0;		
  proc.startip=(addr_t)header.e_entry;
  proc.stackend=(addr_t)((uint)proc.masterrw+pgsize-4096);
  proc.mmu.map_identity();
  
  proc.mmu.map_large(proc.masterrw,proc.masterrw,0x87,1);
  proc.mmu.map_large(to,to,0x85,1);
//EMERGENCY STACK INIT
  memcpy(proc.masterrw+pgsize-4,(void*)&proc.sharedrw,4);
  memcpy(proc.masterrw+pgsize-8,(void*)&proc.masterrw,4);
  memcpy(proc.masterrw+pgsize-12,(void*)&proc.masterro,4);
  memcpy(proc.masterrw+pgsize-4,(void*)&proc.rank,4);
*/
  // 
  // insert your code here
  //

}




static inline void ring3_step(preempt_t& preempt, process_t& proc, dev_lapic_t& lapic){

  //
  //insert your code here
  //

}

static inline void ring3_step_done(process_t& proc, dev_lapic_t& lapic){
  
  //
  //insert your code here
  //

}



