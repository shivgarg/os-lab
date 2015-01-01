#include "apps/labs.h"

#include "x86/except.h"

//
// app step
//
static void apps_loop_step(int rank, addr_t& main_stack, apps_t& apps, uint32_t* systemcallmmio){


}



//
// reset
//
extern "C" void apps_reset(int rank, apps_t& apps, bitpool_t& pool4k, uint32_t* systemcallmmio){


  asm volatile ("nop ");

  uint32_t ret1;
  uint32_t ret2;
  uint32_t ret3;
  xsyscall(systemcallmmio, 0x9, 0,0,0, ret1,ret2,ret3);
  hoh_debug("Allocated at: "<<ret1);

  addr_t varray=addr_t(2<<30);
  if(ret1!=uintptr_t(varray)){
    xsyscall(systemcallmmio, 0x6, ret1,uintptr_t(varray),0, ret1, ret2, ret3);
  }
  
  hoh_debug("Testing ....");
  *varray=0;
  hoh_debug("Testing done....");

  hoh_debug("Bomb..");
  *(varray+(4<<20))=0;
  hoh_debug("Bomb defused..");


  //..... print return values




}


//
// main loop
//
extern "C" void apps_loop(int rank, addr_t* pmain_stack, apps_t* papps, uint32_t* systemcallmmio){
  addr_t& main_stack = *pmain_stack;
  apps_t& apps       = *papps;

  uint32_t esp;
  asm volatile ("mov %%esp,%0":"=r"(esp)::);

  for(;;){
    apps_loop_step(rank, main_stack, apps, systemcallmmio);
  }

}

