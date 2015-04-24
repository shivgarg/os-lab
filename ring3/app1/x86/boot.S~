
#define VAL(x)    (x-_start)(%ebx)
#define UNVAL(x)  (_start-x)(%ebx)
#define PC_RELATIVE(x,y)   leal VAL(x),y
#define UNPC_RELATIVE(x,y) leal UNVAL(x),y

.text
.globl start, _start



.align 16
.code32
start:
_start:
    jmp main_start
    nop
    nop
    jmp user_exception
    nop
    nop



#
#stack layout
#
#12: sharedrw
#8:  masterrw
#4:  masterro
#0:  rank
main_start:
    movl %esp, %ebp
    call 1f
1:
    popl %ebx
    UNPC_RELATIVE(1b,%ebx)

    #-4:  28: pcore
    #-8:  24: pstack
    #-12: 20: ret_core
    #-16: 16: ret_stack
    #-20: 12: sharedrw
    #-24:  8: masterrw
    #-28:  4: masterro
    #-32:  0: rank
    subl $32, %esp

    leal  -4(%ebp), %esi
    movl  %esi, -12(%ebp)

    leal  -8(%ebp), %esi
    movl  %esi, -16(%ebp)

    movl  12(%ebp), %esi
    movl  %esi, -20(%ebp)

    movl  8(%ebp), %esi
    movl  %esi, -24(%ebp)

    movl  4(%ebp), %esi
    movl  %esi, -28(%ebp)

    movl  0(%ebp), %esi
    movl  %esi, -32(%ebp)

    PC_RELATIVE(core_mem_init, %esi)
    call *%esi


    movl -4(%ebp), %eax  #core
    movl -8(%ebp), %ecx  #stack
    addl $32, %esp



    #switch stack
    movl %ecx, %esp    #stack
    movl $0,   %ebp

    #-4:  12: $0
    #-8:   8: $0
    #-12:  4: core
    #-16:  0: rank
    subl $16, %esp

    movl  $0,   12(%esp)
    movl  $0,    8(%esp)
    movl  %eax,  4(%esp)
    movl  $0  ,  0(%esp)

    PC_RELATIVE(core_reset, %esi)
    call *%esi

    PC_RELATIVE(core_loop, %esi)
    call *%esi

    addl $16, %esp

1:
    nop
    leal VAL(1b), %esi
    jmp *%esi



#
#stack layout
#
#32: cr2
#28: %old_eip
#24: %old_esp
#20: errorcode
#16: num
#12: sharedrw
#8:  masterrw
#4:  masterro
#0:  rank
user_exception:


#
# -4: ebx
# -8: ebp
#-12: eax
#-16: ecx
#-20: edx
#
#
    pushl %ebx
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx


    call 1f
1:
    popl %ebx
    UNPC_RELATIVE(1b,%ebx)

    movl $0, %ebp


    leal 20(%esp), %edx
    pushl %edx

    PC_RELATIVE(handle_user_fault, %eax)
    call *%eax

    popl %edx

    #test %eax, %eax
    #jnz 1f

    # restore stack
    movl 24(%edx), %eax
    movl %eax, %esp

    # push eip : user stack!
    movl 28(%edx), %eax
    pushl %eax

    # restore registers
    movl  -4(%edx), %ebx
    movl  -8(%edx), %ebp
    movl -12(%edx), %eax
    movl -16(%edx), %ecx
    movl -20(%edx), %edx

    ret


1:
    # save stack




1:
    nop
    leal VAL(1b), %eax
    jmp *%eax





