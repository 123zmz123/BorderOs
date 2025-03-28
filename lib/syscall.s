.section .text
.global writeu
.global sleepu

writeu:
    // now we in
    sub sp, sp, #16
    mov x8, #0

    stp x0, x1, [sp]
    mov x0, #2
    mov x1, sp
    svc #1234

    add sp, sp, #16
    ret
    
sleepu:
    // trapframe->x8 = 1 for syscall number
    sub sp, sp, #8
    mov x8, #1

    str x0, [sp]

    mov x0, #1
    // trapframe pointer as parameter
    mov x1, sp
    svc #1234

    add sp, sp, #8
    ret