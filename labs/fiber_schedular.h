#pragma once


#include "labs/shell.h"
#include "util/fiber.h"
#include "apps/labs.h"
#include "labs/preempt.h"

#include "devices/lapic.h"

//static void apps_loop_step(int rank, addr_t& main_stack, preempt_t& preempt, apps_t& apps, dev_lapic_t& lapic);
void shell_step_fiber_schedular(shellstate_t& shellstate, addr_t& main_stack,preempt_t& preempt,addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size,dev_lapic_t& lapic);


