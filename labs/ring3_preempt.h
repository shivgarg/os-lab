#pragma once

#include "util/config.h"


//
// preempt_t
//
//
//

#define _ring3_preempt(_name, _f)   \
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
         "  pushl %eax						 \n\t"\
         "  movl %gs:32, %eax				 \n\t"\
         "  #movl (%eax), %eax				\n\t"\
         "  movl %edi , 0x400(%eax)       \n\t"\
         "  movl %esi , 0x404(%eax)       \n\t"\
         "  movl %ebp , 0x408(%eax)       \n\t"\
         "  movl %ebx , 0x410(%eax)       \n\t"\
         "  movl %edx , 0x414(%eax)       \n\t"\
         "  movl %ecx , 0x418(%eax)       \n\t"\
         "  fxsave (%eax)              \n\t"\
         "  popl %ebx                       \n\t"\
         "  movl %ebx , 0x41c(%eax)       \n\t"\
         "  popl %ebx						\n\t"\
         "  popl %ebx						\n\t"\
         "  popl %ebx						\n\t"\
         "  movl %ebx, 0x420(%eax)			\n\t"\
         "	popl %ebx						\n\t"\
         "	popl %ebx						\n\t"\
         "	movl %ebx, 0x424(%eax)			\n\t"\
         "	popl %ebx						\n\t"\
         "	movl %ebx, 0x40c(%eax)			\n\t"\
         "  movl %gs:28, %ebx				\n\t"\
         "	movl %ebx, %cr3					\n\t"\
         "  sti	                              \n\t"\
         "                                   \n\t"\
         "  movl $0, %ebp                    \n\t"\
         "  pushl %gs:0                      \n\t"\
         "  pushl %gs:4                      \n\t"\
         "  call core_loop                   \n\t"\
         );                                        \




