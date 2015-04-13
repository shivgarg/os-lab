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
         "  pushl %eax		             \n\t"\
	 "  pushl %ebx			     \n\t"\
         "  movl %gs:0x20, %eax              \n\t"\
	 "  movl 0x430(%eax), %ebx	     \n\t"\
         "  addl 0x3ffffc, %ebx              \n\t"\
         "  movl %cr2, (%ebx)                \n\t"\
         "  pushl %ecx                       \n\t"\
         "  movl -0x14(%esp),%ecx            \n\t"\
         "  movl %ecx,-0x4(%ebx)             \n\t"\
         "  movl -0x40(%esp),%ecx            \n\t"\
         "  movl %ecx,-0x8(%ebx)             \n\t"\
         "  movl (%esp), %ecx                \n\t"\
         "  movl %ecx,-0xc(%ebx)             \n\t"\
         "  movl 0x4(%esp),%ecx              \n\t"\
         "  movl %ecx,-0x10(%ebx)            \n\t"\
         "  movl 0x434(%eax), %ecx           \n\t"\
         "  movl %ecx, -0x14(%ebx)           \n\t"\
         "  movl 0x430(%eax), %ecx           \n\t"\
         "  movl %ecx, -0x18(%ebx)           \n\t"\
         "  movl 0x42c(%eax), %ecx           \n\t"\
         "  movl %ecx,-0x1c(%ebx)            \n\t"\
         "  movl 0x428(%eax), %ecx           \n\t"\
         "  movl %ecx,-0x20(%ebx)            \n\t"\
         "  popl %ecx                        \n\t"\
         "  popl %ebx                        \n\t"\
         "  popl %eax                        \n\t"\
         "  pushl $0x23                      \n\t"\
	 "  pushl 
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


