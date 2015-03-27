#pragma once

#include "util/config.h"


//
// upcall
//
//
//




#define _ring3_upcall(_name, _f)                  \
         __asm(                                   \
         "  .text                            \n\t"\
         " " STR(_name) ":                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "  #insert your code here           \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "                                   \n\t"\
         "  pushl %edx                       \n\t"\
         "  pushl %ecx                       \n\t"\
         "  pushl %eax                       \n\t"\
         "  call " STR(_f) "                 \n\t"\
         "  popl  %eax                       \n\t"\
         "  popl  %ecx                       \n\t"\
         "  popl  %edx                       \n\t"\
         "                                   \n\t"\
         "  jmp iret_toring3                 \n\t"\
         )                                        \


