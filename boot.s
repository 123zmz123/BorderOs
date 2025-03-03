.section .text
.global start // used by the linker

start:
    mrs x0, mpidr_el1 // read the cpu infor to reg_x0
    and x0, x0, #3 // reg_x0 &= 0b11
    cmp x0, #0 // if reg_x0 == 0x0 --> cpu is needed
    beq kernel_entry

end:
    b end // infinite loop if code executed at here. cpu_id !=0 will execute at here

kernel_entry:
    /*if current exception level is not 2 then branch to end*/
    mrs x0, currentel
    lsr x0, x0, #2
    cmp x0, #2
    bne end

    //write 0 to sctrl_el1 reg
    msr sctlr_el1, xzr
    // el1 will run at aarch64
    mov x0, #(1 << 31)
    msr hcr_el2, x0

    // el_mode = 0101
    mov x0, #0b1111000101
    msr spsr_el2, x0
    // el2 exception return address=el1_entry
    adr x0, el1_entry
    msr elr_el2, x0
    // trigger exception pstate = spsr_el2
    eret

el1_entry:
    mov sp, #0x80000 // stack pointer = 0x80000

    bl setup_vm
    bl enable_mmu
    
    /*memset(bss_start,0,bss_start-bss_end*/
    ldr x0, =bss_start
    ldr x1, =bss_end
    sub x2, x1, x0
    mov x1, #0
    bl memset

    // setup el1 vector table address
    ldr x0, =vector_table
    msr vbar_el1, x0

    // already were virt address
    mov x0, #0xffff000000000000
    add sp, sp, x0
    ldr x0, =KMain
// jump to the Kmain(which defined in c)
    blr x0
    b end
