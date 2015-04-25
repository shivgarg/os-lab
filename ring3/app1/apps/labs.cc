#include "apps/labs.h"

#include "x86/except.h"

//
// app step
//

void for_each();
static void apps_loop_step(int rank, addr_t& main_stack, apps_t& apps, uint32_t* systemcallmmio){

	hoh_debug("looping");
	for_each();
	uint32_t q;
	xsyscall(systemcallmmio,0x1,0,0,0,q,q,q);
}

uint32_t f2(uint8_t x,uint8_t y,uint8_t z)
{
	//hoh_debug("aa "<<(2<<30)+4*(((uint32_t)x)*256u*256u+((uint32_t)y)*256+z));
	//hoh_debug("aqdqwdqwdqqq");
	return *(uint32_t*)((uint32_t)(2<<30)+4*(((uint32_t)x)*256u*256u+((uint32_t)y)*256u+z));
}

uint32_t sum_neighbours(int x, int y, int z){
		//hoh_debug("X "<<x<<"Y "<<y<<"Z "<<z);
			int d=64;
      uint32_t sum=0;
      if((z/32)%2==1){
            for(int i=-d; i<d; i++){
              for(int j=-d; j<d; j++){
                 for(int k=-d; k<d; k++){
		
                   sum += f2((uint8_t)(x+i), (uint8_t)(y+j), (uint8_t)(z+k));
    
                 }
              }
            }
          }
      else
      {
          for(int i=d-1; i>-d-1; i--){
              for(int j=-d; j<d; j++){
                 for(int k=-d; k<d; k++){
    
                   sum += f2((uint8_t)(x+i), (uint8_t)(y+j), (uint8_t)(z+k));
    
                 }
              }
            }
      }
	
	return sum;
        }
/*
uint32_t weightedsum_neighbours(uint8_t x, uint8_t y, uint8_t z){
           size_t sum=0;
            for(int i=-d; i<d; i++){
              for(int j=-d; j<d; j++){
                 for(int k=-d; k<d; k++){
                   sum += w(i,j,k,d) * f2(x+i, y+j, z+k);
                 }
              }
            }
        }
*/
void for_each()
{
	hoh_debug("in for each start");
	uint32_t a=0;
	for(int x=0;x<256;x+=32){
		//hoh_debug("X  "<<a);
		for(int y=0;y<256;y+=32){
			//hoh_debug("Y "<<a<< " "<< x << " "<< y );
			for(int z=0;z<256;z+=32)
			{	

			//hoh_debug("being called"<<a<<" "<<x <<" "<< y << " "<<z);
        uint32_t tmp=sum_neighbours(x,y,z);
        //hoh_assert(tmp==0x200000,"PANIC . VALUE NOT AS EXPECTED");
				 a+=tmp;
        // hoh_debug("sum value "<< tmp);
				
			}
				}
}
	hoh_debug("as "<<a);
}

//
// reset
//
extern "C" void apps_reset(int rank, apps_t& apps, bitpool_t& pool4k, uint32_t* systemcallmmio){

hoh_debug("in here");
  asm volatile ("nop ");

/*  uint32_t ret1;
  uint32_t ret2;
  uint32_t ret3;
  xsyscall(systemcallmmio, 0x9, 0,0,0, ret1,ret2,ret3);
  hoh_debug("Allocated at: "<<ret2);

  addr_t varray=addr_t(2<<30);
  if(ret1!=uintptr_t(varray)){
    xsyscall(systemcallmmio, 0x6, ret2,uintptr_t(varray),0, ret1, ret2, ret3);
  }
  
  hoh_debug("Testing ....");
  *varray=0;
  hoh_debug("Testing done....");

  hoh_debug("Bomb..");
  *(varray+(4<<20))=0;*/
  hoh_debug("Comb defused..");


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

