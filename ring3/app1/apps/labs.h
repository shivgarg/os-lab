#pragma once

#include "util/debug.h"
#include "util/bitpool.h"

struct apps_t{
	int bitvector[16];
	int count;
	apps_t(){
		count=0;
		for(int i=0;i<16;i++)
			bitvector[i]=0;
	}
};


extern "C" void apps_reset(int rank, apps_t& apps, bitpool_t& pool4k, uint32_t* systemcallmmio);
extern "C" void apps_loop(int rank, addr_t* main_stack, apps_t* apps, uint32_t* systemcallmmio);

