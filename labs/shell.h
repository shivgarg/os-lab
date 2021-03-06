#pragma once
#include "util/config.h"


	struct pair_int{
		bool * done;
		int * run_inst;
		int fun;
		char * ret;
		int arg;
	};
struct shellstate_t{
	int leftarea=0;
	char inp[22][25];
	int keys=0;
	int iter=0;
	int index;
	

	int inp_ind;
	int char_ind;
	int change;
	
	int left;
	int output;
	


	bool done=true;
	bool init_coroutine=false;
	int coroutine_out;
	int coroutine_arg=0;
	


	bool init_fiber=false;
	int fiber_out;
	int fiber_arg;
	bool fiber_done;
	


	int scheduler_assign;
	int scheduler_run;
	int scheduler_arg;
	int scheduler_out;
	bool schd_slots[10];
	int first[5];
	int fib_inst[5];
	int fib_arg[5];
	int run_instances[5];

	pair_int fibs[5];

};

struct renderstate_t{
	int left;
	int leftarea=0;
	int keys=0;
	char inp[22][25];
	int iter;
	int inp_ind;
	int change;	
	bool done;
	int coroutine_out;
	bool fiber_done;	
	int fiber_out;	
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

