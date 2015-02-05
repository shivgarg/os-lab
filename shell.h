#pragma once
#include "util/config.h"

struct shellstate_t{
	int leftarea=0;
	char inp[100];
	int keys=0;
	int output=0;
	int out1=0;
	char out2[100];
	int iter=0;
	int ind=0;

};

struct renderstate_t{
	
	int leftarea=0;
	int keys=0;
	char inp[100];
	int output=0;
	int out1=0;
	char out2[100];
	int iter;
	int ind;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

