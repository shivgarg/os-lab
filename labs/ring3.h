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
  hoh_assert(header.e_entry<pgsize,"Start IP outside page");
  addr_t to = alloc(pool4M); 

// ELF LOADED
  for(int i=0;i<prog_num;i++)
  {
	if(prog_header[i].p_type!=PT_LOAD){
		continue;
	}
	hoh_assert(prog_header[i].p_filesz<=prog_header[i].p_memsz,"Mem size is less than file size");
	hoh_assert(prog_header[i].p_memsz<=pgsize,"ELF larger than 4MB");
	hoh_assert(prog_header[i].p_filesz<fromsize,"Header filesz greater than ELF Size");
	hoh_assert(prog_header[i].p_offset+prog_header[i].p_filesz<fromsize, "Header greater than fromsize");
	hoh_assert(prog_header[i].p_vaddr+prog_header[i].p_memsz<=pgsize,"Pgsize exceeded while copying executable");
	hoh_assert(prog_header[i].p_vaddr<pgsize,"Vaddr greater than pgsize");
	if(prog_header[i].p_align>0)
		hoh_assert(prog_header[i].p_offset%prog_header[i].p_align==prog_header[i].p_vaddr%prog_header[i].p_align,"Not aligned");
	hoh_assert(prog_header[i].p_align==0 || ((prog_header[i].p_align!=0) && !(prog_header[i].p_align & (prog_header[i].p_align-1))),"p_align not 0 or power of 2");
	hoh_assert(prog_header[i].p_offset<fromsize,"Offset for laoding outside ELF");
	memcpy(to+prog_header[i].p_vaddr,(void *)(prog_header[i].p_offset+from),prog_header[i].p_filesz);
	memset(to+prog_header[i].p_vaddr+prog_header[i].p_filesz,0,prog_header[i].p_memsz-prog_header[i].p_filesz);
  }
	
 //PROC INITIALISATION
  proc.eip=(uint)(header.e_entry+to);
  proc.eflags=0x3200;
  proc.rank=0;
  proc.masterro=0;
  proc.masterrw=alloc(pool4M);
  proc.sharedrw=0;		
  proc.startip=(addr_t)(header.e_entry+to);
  proc.stackend=(addr_t)((uint)proc.masterrw+pgsize-4096);
  proc.mmu.map_identity();
  proc.esp=(uint32_t)proc.stackend+4096-16; 
  proc.state=1;
  hoh_debug("PROC ESP"<< proc.esp);

  proc.mmu.map_large(proc.masterrw,proc.masterrw,0x87,1);
  proc.mmu.map_large(to,to,0x85,1);
//SYSTEM CALL AREA ZEROED OUT
  memset(proc.masterrw,0,64);

//EMERGENCY STACK INIT
  memcpy(proc.masterrw+pgsize-4,(void*)&proc.sharedrw,4);
  memcpy(proc.masterrw+pgsize-8,(void*)&proc.masterrw,4);
  memcpy(proc.masterrw+pgsize-12,(void*)&proc.masterro,4);
  memcpy(proc.masterrw+pgsize-16,(void*)&proc.rank,4);
  // 
  // insert your code here
  //

}




static inline void ring3_step(preempt_t& preempt, process_t& proc, dev_lapic_t& lapic){

  //
  //insert your code here
  //
	if(proc.state==1)
	{
		proc.eflags=(proc.eflags & ~(3u<<12)) | (proc.iopl<<12);
		lapic.reset_timer_count(10000000);
		asm volatile(							 \
			" pushl %%esi 					\n\t"
			"cli							\n\t"\
			"movl %%cr3 ,%%esi 					\n\t"\
			"movl %%esi ,%%gs:28					\n\t"\
			"movl %%edx ,%%esi 					\n\t"\
			"movl %%esi ,%%gs:32					\n\t"\
			" popl %%esi 						\n\t"
			:							 \
			:"d" (&proc) 					 \
			);
		proc.mmu.reset();
		asm volatile(								 \
  			" movw $32, %%ax                    				\n\t"\
    			" movw %%ax, %%ds                    				\n\t"\
    			" movw %%ax, %%es                    				\n\t"\
    			" movw $0,  %%ax                    				\n\t"\
    			" movw %%ax, %%fs                    				\n\t"\
 			" movw %%ax, %%gs                    				\n\t"\
		 	" fxrstor (%0)							\n\t"\
			" movl " STR(process_offset_edi) "(%0), %%edi 			\n\t"\
			" movl " STR(process_offset_esi) "(%0), %%esi 			\n\t"\
			" movl " STR(process_offset_ebp) "(%0), %%ebp 			\n\t"\
			" movl " STR(process_offset_ebx) "(%0), %%ebx 			\n\t"\
			" movl " STR(process_offset_eax) "(%0), %%eax 			\n\t"\
			" movl " STR(process_offset_ecx) "(%0), %%ecx 			\n\t"\
			" pushl $0x23 							\n\t"\
			" pushl " STR(process_offset_esp) "(%0)				\n\t"\
			" pushl " STR(process_offset_eflags) "(%0)			\n\t"\
			" pushl $0x1b 							\n\t"\
			" pushl " STR(process_offset_eip) "(%0) 			\n\t"\
			" movl " STR(process_offset_edx) "(%0), %%edx 			\n\t"\
			" iretl 							\n\t"\
			:								 \
			:"d" (&proc)							 \
		 	:								 \
		 	);
	}									
}

static inline void ring3_step_done(process_t& proc, dev_lapic_t& lapic){
  
  //
  //insert your code here
  //
	lapic.reset_timer_count(0);


}



