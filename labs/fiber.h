#pragma once

#include "labs/shell.h"
#include "util/fiber.h"
#include "apps/labs.h"
#include "labs/preempt.h"

#include "devices/lapic.h"

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack,preempt_t& preempt, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize,dev_lapic_t& lapic);

