.section .text
.global writeu

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
    