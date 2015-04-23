#pragma once

#include "apps/labs.h"

struct config_t{

};


struct core_t{
public:
  uint32_t    syscallmmio[16];
  core_t*     self;
  int         rank;
  addr_t      main_stack;
  addr_t      main_stackbegin;
  size_t      main_stacksize;
  addr_t      main_stackend_cached;

  size_t      magic;  //=0xface600d;
  bitpool_t   pool4K;
  apps_t      apps;
  config_t    config;

public:
  core_t(int trank,const bitpool_t& tpooltmp, addr_t tstack, size_t tstacksize, const config_t& tconfig) : 
    self(this),
    rank(trank),
    main_stackbegin(tstack), // XXX: reset the pool too
    main_stacksize(tstacksize),
    main_stackend_cached(tstack+tstacksize),
    pool4K(tpooltmp),
    config(tconfig),
    apps()
  {
    main_stack = addr_t(0xfacebaad); //to tell you that their value is random
    magic=0xface600d;

    hoh_debug("Ring3 core: "<<rank<<" "<<uintptr_t(self));
  }
};



