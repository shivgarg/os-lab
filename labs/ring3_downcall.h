#pragma once


#include "util/ring3.h"
#include "devices/lapic.h"
#include "util/bitpool.h"

static inline void ring3_downcall(process_t& proc, dev_lapic_t& lapic, bitpool_t& pool4M){

  uint32_t* systemcall_mmio = reinterpret_cast<uint32_t*>(proc.masterrw);
  uint32_t fnum             = systemcall_mmio[1];  //read fnum first.

  if(fnum==0){ //make sure you check fnum.
    return;
  }

  uint32_t farg1=systemcall_mmio[2];
  uint32_t farg2=systemcall_mmio[3];
  uint32_t farg3=systemcall_mmio[4];


  hoh_debug("Systemcall: "<<fnum<<": "<<farg1<<","<<farg2<<","<<farg3);

  uint32_t fret1=0;
  uint32_t fret2=0;
  uint32_t fret3=0;

  switch(fnum){
  default:{
           hoh_debug("Invalid system call. Killing the application");
           //proc->state=PROC_DONE;
           //free resources
          }break;
  case 0: { //nop: don't do anything
          }break;
  case 1: { //done
            //proc->state=PROC_DONE;
            //free resources
          }break;
  case 2: {  //mmio_read
          }break;
  case 3: {  //mmio_write
          }break;
  case 4: {  //io_read
          }break;
  case 5: {  //io_write
          }break;
  case 6: {  //mmu_swapva
             //align va1 and va2 using prev_align
             //swap using: proc.mmu.get/set()
          }break;
  case 7: {  //mmu_mapmmio
             //align va using prev_align
             //proc.mmu.map
          }break;
  case 8: {  //mmu_mappmio
             //proc.iopl=3; and make sure in your ring3_step you respect this variable
          }break;
  case 9: {  //addr_t x=alloc(pool4M);
             //unused_page = 0;
             //unused_page = find a page in VA_RANGE. ie: [ (0x2<<30) , (0x3<<30) )
             //if you're able to find a page: 
             //   proc.mmu.map_large(unused_page,x,0x87);
          }break;
  }


  if(fnum!=0){
    // do not modify the arguments if fnum is zero.
    systemcall_mmio[2]=fret1;
    systemcall_mmio[3]=fret2;
    systemcall_mmio[4]=fret3;
    systemcall_mmio[1]=0; //modify this last. ZERO means systemcall done.
  }

  hoh_debug("Systemcall: "<<fnum<<": "<<farg1<<","<<farg2<<","<<farg3<<" RESULT: "<<fret1<<","<<fret2<<","<<fret3);

}

