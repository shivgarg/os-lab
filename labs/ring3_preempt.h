#pragma once

#include "util/config.h"


//
// preempt_t
//
//
//

#define _ring3_preempt(_name, _f) \
         __asm(                                   \
         "  .text                            \n\t"\
         " " STR(_name) ":                   \n\t"\
         "  pushl %edx                       \n\t"\
         "  pushl %ecx                       \n\t"\
         "  pushl %eax                       \n\t"\
         "  call " STR(_f) "                 \n\t"\
         "  popl  %eax                       \n\t"\
         "  popl  %ecx                       \n\t"\
         "  popl  %edx                       \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "  # insert your code here          \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "  movl $0, %ebp                    \n\t"\
         "  pushl %gs:0                      \n\t"\
         "  pushl %gs:4                      \n\t"\
         "  call core_loop                   \n\t"\
         "                                   \n\t"\
         )                                        \



