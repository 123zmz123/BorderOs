.section .text
.global start // used by the linker

start:
    mrs x0, mpidr_el1 // read the cpu infor to reg_x0
    and x0,x0,#3 // reg_x0 &= 0b11
    cmp x0, #0 // if reg_x0 == 0x0 --> cpu is needed
    beq kernel_entry

end:
    b end // infinite loop if code executed at here. cpu_id !=0 will execute at here

kernel_entry:
    mov sp, #0x80000 // stack pointer = 0x80000
    bl KMain // jump to the Kmain(which defined in c)
    b end
