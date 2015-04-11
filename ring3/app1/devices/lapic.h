#pragma once

#include "generated/xapic.dev.h"
#include "devices/config.h"

class dev_lapic_t{
  public:
    dev_lapic_t(addr_t base){
    }
  public:
    void dump(){
    }
    void reset_timer_count(uint32_t count){
    }
    void reset_timer_count(uint32_t count, xapic_divide_t div_val){
    }
};



