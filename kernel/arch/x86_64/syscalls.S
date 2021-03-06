.section .text

.macro save_context
    # save registers to CpuState struct, in the right order that reflects CpuState struct fields
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11   // usermode RFLAGS is stored here
    push %r10
    push %r9
    push %r8
    push %rbp
    push %rdi
    push %rsi
    push %rdx
    push %rcx   // usermode RIP is stored here
    push %rbx
    // push %rax; rax will contain syscall result value so no need to remember it
    // push %cr3; not needed for syscall as we always come back to the same task(as long as kernel is non-preemptive)

    sub $256, %rsp
    movdqu %xmm15, (%rsp)
    movdqu %xmm14, 16(%rsp)
    movdqu %xmm13, 32(%rsp)
    movdqu %xmm12, 48(%rsp)
    movdqu %xmm11, 64(%rsp)
    movdqu %xmm10, 80(%rsp)
    movdqu %xmm9, 96(%rsp)
    movdqu %xmm8, 112(%rsp)
    movdqu %xmm7, 128(%rsp)
    movdqu %xmm6, 144(%rsp)
    movdqu %xmm5, 160(%rsp)
    movdqu %xmm4, 176(%rsp)
    movdqu %xmm3, 192(%rsp)
    movdqu %xmm2, 208(%rsp)
    movdqu %xmm1, 224(%rsp)
    movdqu %xmm0, 240(%rsp)
.endm

.macro restore_context
    # restore registers from CpuState struct
    movdqu (%rsp), %xmm15
    movdqu 16(%rsp), %xmm14
    movdqu 32(%rsp), %xmm13
    movdqu 48(%rsp), %xmm12
    movdqu 64(%rsp), %xmm11
    movdqu 80(%rsp), %xmm10
    movdqu 96(%rsp), %xmm9
    movdqu 112(%rsp), %xmm8
    movdqu 128(%rsp), %xmm7
    movdqu 144(%rsp), %xmm6
    movdqu 160(%rsp), %xmm5
    movdqu 176(%rsp), %xmm4
    movdqu 192(%rsp), %xmm3
    movdqu 208(%rsp), %xmm2
    movdqu 224(%rsp), %xmm1
    movdqu 240(%rsp), %xmm0
    add $256, %rsp

    // pop %cr3; not needed for syscall
    // pop %rax; dont override the on_syscall result value
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %rbp
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
.endm

.extern on_syscall
.extern kernel_stack_top
.global handle_syscall
handle_syscall:
    save_context

    # remember user stack pointer
    mov %rsp, (user_rsp)

    # switch to kernel stack
    mov $kernel_stack_top, %rsp

    # set 5 syscalls params as function args no. 2, 3, 4, 5, 6
    # see http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64 and https://en.wikipedia.org/wiki/X86_calling_conventions "System V AMD64"
    mov %r8, %r9
    mov %r10, %r8
    mov %rdx, %rcx
    mov %rsi, %rdx
    mov %rdi, %rsi

    # set syscall number as function arg no. 1
    mov %rax, %rdi

    # call C++ syscall handler, result value comes back on %rax
    call on_syscall

    # restore user stack
    mov (user_rsp), %rsp

    restore_context

    sysretq

.section .data
    user_rsp: .quad 0
