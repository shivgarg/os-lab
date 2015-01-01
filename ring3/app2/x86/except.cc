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


extern "C" void handle_user_fault(usertrapframe_t& x){

  hoh_debug("user fault: rank="<<x.rank<<","<<x.masterro<<","<<x.masterrw<<","<<x.sharedrw
                               <<",num="<<x.num<<",ec="<<x.errorcode<<",esp="<<x.old_esp<<",eip="<<x.old_eip
                               <<",cr2="<<x.cr2);

 core_t* pcore=(core_t*) x.masterrw;
 core_t& core = *pcore;

 addr_t varray=addr_t(2<<30);
 hoh_assert(uint32_t(varray)<=x.cr2 && x.cr2<(uint32_t(varray)+16*(4<<20)),"XXX");

 uint32_t ret1;
 uint32_t ret2;
 uint32_t ret3;
 xsyscall(core.syscallmmio, 0x6, (uint32_t)prevalign(addr_t(x.cr2),4<<20), uintptr_t(varray), 0, ret1, ret2, ret3);


}

