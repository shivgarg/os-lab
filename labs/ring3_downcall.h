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
           proc.state=0;
	   fret1=0;
	   free(pool4M,proc.masterrw);
	   free(pool4M,prevalign(proc.startip,0x400000));
           //free resources
          }break;
  case 0: { //nop: don't do anything
		fret1=1;
          }break;
  case 1: { //done
            proc.state=0;
	    fret1=1;
	    free(pool4M,proc.masterrw);
	    free(pool4M,prevalign(proc.startip,0x400000));
            //free resources
          }break;
  case 2: {  //mmio_read
		switch(farg1)
		{
			default:{
					proc.state=0;
					free(pool4M,proc.masterrw);
					free(pool4M,prevalign(proc.startip,0x400000));
					fret1=0;
				}break;
			case 8:	{	
					fret1=1;
					fret2=mmio::read8(farg2,0);
				}break;
			case 16:{
					fret1=1;
					fret2=mmio::read16(farg2,0);
				}break;
			case 32:{
					fret1=1;
					fret2=mmio::read32(farg2,0);
				}break;					
		}
          }break;
  case 3: {  //mmio_write
		swtich(farg1)
		{
			default:{
					proc.state=0;
					free(pool4M,proc.masterrw);
					free(pool4M,prevalign(proc.startip,0x400000));
					fret1=0;
				}break;
			case 8:	{
					fret1=1;
					mmio::write8(farg2,0,farg3);
				}break;
			case 16:{
					fret1=1;
					mmio::write16(farg2,0,farg3);
				}break;
			case 32:{
					fret1=1;
					mmio::write32(farg2,0,farg3);
				}break;
		}
          }break;
  case 4: {  //io_read
		switch(farg1)
		{
			default:{
					proc.state=0;
					free(pool4M,proc.masterrw);
					free(pool4M,prevalign(proc.startip,0x400000));
					fret1=0;
				}break;
			case 8:	{
					fret1=1;
					fret2=io::read8(farg2,0);
				}break;
			case 16:{
					fret1=1;
					fret2=io::read16(farg2,0);
				}break;
			case 32:{
					fret1=1;
					fret2=io::read32(farg2,0);
				}break;
		}
          }break;
  case 5: {  //io_write
		switch(farg1)
		{
			default:{
					proc.state=0;
					free(pool4M,proc.masterrw);	
					free(pool4M,prevalign(proc.startip,0x400000));
					fret1=0;
				}break;
			case 8:	{
					fret1=1;
					io::write8(farg2,0,farg3);
				}break;
			case 16:{
					fret1=1;
					io::write16(farg2,0,farg3);
				}break;
			case 32:{
					fret1=1;
					io::write32(farg2,0,farg3);
				}break;

		}	
          }break;
  case 6: {  //mmu_swapva
             //align va1 and va2 using prev_align
             //swap using: proc.mmu.get/set()
		hoh_assert(farg1>=0x80000000 && farg1<=0xc0000000 && farg2>=0x80000000 && farg2<=0xc0000000,"Outside VA Range");
		fret1=1;
		proc.mmu.swap(prevalign(farg1,0x400000),prevalign(farg2,0x400000));
		
          }break;
  case 7: {  //mmu_mapmmio
             //align va using prev_align
             //proc.mmu.map

		hoh_assert(!(farg1>=0x80000000 && farg1<=0xc0000000),"Mapped Address Outside VA Range");
		fret1=1;
		proc.mmu.map(prevalign(farg1,0x400000),prevalign(farg1,0x400000),0x87);
          }break;
  case 8: {  //mmu_mappmio
             //proc.iopl=3; and make sure in your ring3_step you respect this variable
		proc.iopl=3;
		fret1=1;
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

