#include "apps/labs.h"


//
// app step
//
int i=0;

static void apps_loop_step(int rank, addr_t& main_stack, apps_t& apps){

  asm volatile ("nop ");
  hoh_debug("in ring3");
  i++;
  hoh_debug("I "<< i );

}



//
// reset
//
extern "C" void apps_reset(int rank, apps_t& apps, bitpool_t& pool4k){


}


//
// main loop
//
extern "C" void apps_loop(int rank, addr_t* pmain_stack, apps_t* papps){
  addr_t& main_stack = *pmain_stack;
  apps_t& apps       = *papps;

  uint32_t esp;
  asm volatile ("mov %%esp,%0":"=r"(esp)::);

  for(;;){
    apps_loop_step(rank, main_stack, apps);
  }

}

