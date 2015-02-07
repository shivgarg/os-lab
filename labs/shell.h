#pragma once
#include "util/config.h"

struct shellstate_t{
	int leftarea=0;
	char inp[200][35];
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
};

struct renderstate_t{
	int left;
	int leftarea=0;
	int keys=0;
	char inp[200][35];
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

