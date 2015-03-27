#pragma once

#include "util/debug.h"
#include "util/bitpool.h"

struct apps_t{

};


extern "C" void apps_reset(int rank, apps_t& apps, bitpool_t& pool4k);
extern "C" void apps_loop(int rank, addr_t* main_stack, apps_t* apps);

