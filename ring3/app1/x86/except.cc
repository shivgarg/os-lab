#include "x86/except.h"
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

}

