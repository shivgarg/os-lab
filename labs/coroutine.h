#pragma once

#include "labs/shell.h"
#include "util/coroutine.h"

// state for your coroutine implementation:
struct f_t{

		int i,j,k;

};


void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals);


