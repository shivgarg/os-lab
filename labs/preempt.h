#pragma once

#include "util/config.h"


//
// preempt_t : State for your timer/preemption handler
//
// Note:
//  We've one instance of core_t per each core.
//  To access this instance, you need to use %gs:0
//  (The entire kernel doesn't have any global/static variables)
//
// %gs:core_offset_preempt will point to start of preempt_t instance
//
// for example: 
// %gs:0 will return pointer to core_t
// %gs:core_offset_mainstack will return core_t::main_stack
// %gs:core_offset_preempt+0 will return core_t::saved_stack
//
// etc.
//
// See Definition of core_t in x86/main.h
//

struct preempt_t{
  // your data structure, if any
  addr_t *saved_stack; //feel free to change it - provided as an example
  addr_t kernel_pgt;
  addr_t proc;

};


//
// 
// This macro is being called from x86/except.cc
//
//
// _name: label name
// _f   : C function to be called 
//        ex: we may have to do send EOI to LAPIC or PIC etc.
//
#  define  _ring0_preempt(_name,_f)            \
  __asm(                                       \
      "  .text                                                  \n\t"\
      " " STR(_name) ":                                         \n\t"\
      "  pushl %edx                                             \n\t"\
      "  pushl %ecx                                             \n\t"\
      "  pushl %eax                                             \n\t"\
      "  call " STR(_f) "                                       \n\t"\
      "  popl  %eax                                             \n\t"\
      "  popl  %ecx                                             \n\t"\
      "  popl  %edx                                             \n\t"\
      "                                                         \n\t"\
      "  # insert your code here                                \n\t"\
      "   cmpl $0, %gs:24                                       \n\t"\
      "   je ret_i						\n\t"\
      "   pushl %eax                                            \n\t"\
      "   pushl %ebx                                            \n\t"\
      "   pushl %ecx                                            \n\t"\
      "   pushl %edx                                            \n\t"\
      "   pushl %ebp                                            \n\t"\
      "   pushl %esi                                            \n\t"\
      "   pushl %edi                                            \n\t"\
      "   movl %esp, %eax 				        \n\t"\
      "   andl $0xf,%eax 		  		        \n\t"\
      "   andl $0xffffff0,%esp                                  \n\t"\
      "   subl $512,%esp				        \n\t"\
      "   fxsave (%esp)				                \n\t"\
      "   pushl %eax 					        \n\t"\
      "   pushl $1f                                             \n\t"\
      "   movl %gs:" STR(core_offset_preempt) ", %eax           \n\t"\
      "   movl %esp, (%eax)                                     \n\t"\
      "   movl %gs:" STR(core_offset_mainstack) ", %esp         \n\t"\
      "   ret                                                   \n\t"\
      "  1:                                                     \n\t"\
      "   popl %eax                                             \n\t"\
      "   fxrstor (%esp)				        \n\t"\
      "   addl $512,%esp                                        \n\t"\
      "   addl %eax,%esp 				        \n\t"\
      "   popl %edi                                             \n\t"\
      "   popl %esi                                             \n\t"\
      "   popl %ebp                                             \n\t"\
      "   popl %edx                                             \n\t"\
      "   popl %ecx                                             \n\t"\
      "   popl %ebx                                             \n\t"\
      "   popl %eax                                             \n\t"\
      "	  #sti													\n\t"\
      " ret_i:                                                  \n\t"\
      "   jmp iret_toring0                                      \n\t"\
      )                                        \


