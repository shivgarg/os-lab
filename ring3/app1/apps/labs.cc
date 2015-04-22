#include "apps/labs.h"


//
// app step
//



void test_systemcall(uint32_t* systemcallmmio,uint32_t arg,uint32_t a,uint32_t b,uint32_t c){

  uint32_t fnum=arg;
  uint32_t arg1=a;
  uint32_t arg2=b;
  uint32_t arg3=c;

  //.....

  systemcallmmio[2]=arg1;
  systemcallmmio[3]=arg2;
  systemcallmmio[4]=arg3;
  systemcallmmio[1]=fnum; //write this field at the end.

  hoh_debug("Shell Before making system call");
  asm volatile("int $0x48":::"memory");
  hoh_debug("Shell After making system call");

  hoh_assert(systemcallmmio[1]==0,"XXX");
  uint32_t ret1=systemcallmmio[2];
  uint32_t ret2=systemcallmmio[3];
  uint32_t ret3=systemcallmmio[4];

 hoh_debug("RETURN VALUE "<< ret1<<" " <<ret2 <<" " <<ret3); 

}











static void apps_loop_step(int rank, addr_t& main_stack, apps_t& apps){

  asm volatile ("nop ");
// asm volatile("int3");
//	for(int i=0;i<10;i++)
//		hoh_debug("hi from ring3 "<<i);
	 int *p=0;
	 hoh_debug("aasda"<<*p);
//hoh_debug("in ring3 wdwfefwef");
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

