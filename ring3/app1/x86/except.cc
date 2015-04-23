#include "x86/except.h"
#include "x86/main.h"
#include "util/util.h"




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


uint32_t f(uint8_t x,uint8_t y,uint8_t z)
{
	//return uint32_t(x+y+z);
	return 1;
}

extern "C" void handle_user_fault(usertrapframe_t& x){

  hoh_debug("user fault: rank="<<x.rank<<","<<x.masterro<<","<<x.masterrw<<","<<x.sharedrw
                               <<",num="<<x.num<<",ec="<<x.errorcode<<",esp="<<x.old_esp<<",eip="<<x.old_eip
                               <<",cr2="<<x.cr2);

 core_t* pcore=(core_t*) x.masterrw;
 core_t& core = *pcore;

 //addr_t varray=addr_t(2<<30);
 hoh_assert((0x80000000<=x.cr2) && (x.cr2<0x80000000+16*(4<<20)),"XXX");
 if(core.apps.count<4)
 {
	hoh_debug("count less than 4");
	uint32_t ret1;
	uint32_t ret2;
	uint32_t ret3;
	xsyscall(core.syscallmmio,0x9,0,0,0,ret1,ret2,ret3);
	core.apps.count++;
	if((uint32_t)ret2!=(uint32_t)prevalign(addr_t(x.cr2),4<<20))
	{
		xsyscall(core.syscallmmio,0x6,(uint32_t)prevalign(addr_t(x.cr2),4<<20),(uint32_t)ret2,0,ret1,ret2,ret3);
	}
	uint32_t last_add=(uint32_t)prevalign(addr_t(x.cr2),4<<20)+4*1024*1024;
	for(uint32_t ind=(uint32_t)prevalign(addr_t(x.cr2),4<<20);ind<last_add;ind+=4)
	{
		*((uint32_t *)ind)=f((ind-0x80000000)/(0x40000),((ind-0x80000000)%0x40000)/0x400,(ind-0x80000000)%0x400);
	}
	for(int i=0;i<16;i++)
	{
		if(core.apps.bitvector[i]>0)
			core.apps.bitvector[i]++;
	}
	core.apps.bitvector[((uint32_t)prevalign(addr_t(x.cr2),4<<20)-0x80000000)/0x400000]=1;
	
 }
 else
 {
 	hoh_debug("swapping out page");
   uint32_t ind=0;
   for(uint32_t i=0;i<16;i++)
   {
	if(core.apps.bitvector[i]>0){
		core.apps.bitvector[i]++;
		if(core.apps.bitvector[i]==5)
			ind=i;
	}
   }
   core.apps.bitvector[ind]=0;
   core.apps.bitvector[((uint32_t)prevalign(addr_t(x.cr2),4<<20)-0x80000000)/0x400000]=1;		
  uint32_t ret1;
  uint32_t ret2;
  uint32_t ret3;
  xsyscall(core.syscallmmio, 0x6, (uint32_t)prevalign(addr_t(x.cr2),4<<20), (uint32_t)(0x80000000+ind*4*1024*1024), 0, ret1, ret2, ret3);
  uint32_t last_add=(uint32_t)prevalign(addr_t(x.cr2),4<<20)+4*1024*1024;
  for(uint32_t indx=(uint32_t)prevalign(addr_t(x.cr2),4<<20);indx<last_add;indx+=4)
  {
       *((uint32_t*)indx)=f((indx-0x80000000)/(0x40000),((indx-0x80000000)%0x40000)/0x400,(indx-0x80000000)%0x400);
  }
 
 }
}

