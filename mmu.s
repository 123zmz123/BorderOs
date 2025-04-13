.equ MAIR_ATTR, (0x44 << 8)
.equ TCR_T0SZ,  (16) 
.equ TCR_T1SZ,  (16 << 16)
.equ TCR_TG0,   (0 << 14)
.equ TCR_TG1,   (2 << 30)
.equ TCR_VALUE, (TCR_T0SZ | TCR_T1SZ | TCR_TG0 | TCR_TG1)
// 2MB
.equ PAGE_SIZE, (2*1024*1024)

.global enable_mmu
.global setup_vm
.global load_pgd
.global read_pgd
// read global page map at el1
read_pgd:
    mrs x0, ttbr0_el1
    ret

load_pgd:
    msr ttbr0_el1, x0
    tlbi vmalle1is
    dsb ish
    isb

    ret


enable_mmu:
    // ttbr1-->kernel ttbr0-->user
    adr x0, pgd_ttbr1    
    msr ttbr1_el1, x0

    adr x0, pgd_ttbr0
    msr ttbr0_el1, x0

    //memroy attribute
    ldr x0, =MAIR_ATTR
    msr mair_el1, x0

    // config the translation behaviour
    ldr x0, =TCR_VALUE
    msr tcr_el1, x0

    // enable mmu
    mrs x0, sctlr_el1
    orr x0, x0, #1
    msr sctlr_el1, x0
    
    ret

setup_vm: // now all the address is still the physical address
setup_kvm:
    adr x0, pgd_ttbr1
    adr x1, pud_ttbr1
    orr x1, x1, #3
    str x1, [x0] // pgd_ttbr1[0]=pud_ttbr1

    adr x0, pud_ttbr1
    adr x1, pmd_ttbr1
    orr x1, x1, #3
    str x1, [x0]

    mov x2, #0x34000000
    adr x1, pmd_ttbr1
    mov x0, #(1 << 10 | 1 << 2 | 1 << 0)

loop1:
    // [pmd_addr] = phy_base_addr(not precise), pmd_addr+=8
    str x0, [x1], #8
    // phy_base_addr+=page_size
    add x0, x0, #PAGE_SIZE
    // phy_base_addr<0x34000000? then  goto loop1
    cmp x0, x2
    blo loop1

    // periphal area map to kernel space
    mov x2, #0x40000000
    mov x0, #0x3f000000

    adr x3, pmd_ttbr1
    // 0x3f000000 >> 21(page size 2^21) << 3(*8) => offset
    lsr x1, x0, #(21 - 3)
    // x1=pmd_ttbr1+offset
    add x1, x1, x3

    orr x0, x0, #1
    orr x0, x0, #(1 << 10)

loop2:
    // [pmd+index]=phy_addr,index+=1
    str x0, [x1], #8
    add x0, x0, #PAGE_SIZE
    // phy_addr < 0x40000000 then goto loop2
    cmp x0, x2
    blo loop2

    adr x0, pud_ttbr1
    add x0, x0, #(1 * 8)
    adr x1, pmd2_ttbr1
    orr x1, x1, #3
    str x1, [x0]

    mov x2, #0x41000000
    mov x0, #0x40000000

    adr x1, pmd2_ttbr1
    orr x0, x0, #1
    orr x0, x0, #(1 << 10)

loop3:
    str x0, [x1], #8
    add x0, x0, #PAGE_SIZE
    cmp x0, x2
    blo loop3    



setup_uvm:
    // user space setup 1st mapping
    // vir        <----> phy
    // 0~0x200000 <----> 0~0x200000
    // exp 0x000_000_000_200000--> index_000 index_0000 index_0000
    adr x0, pgd_ttbr0
    adr x1, pud_ttbr0
    orr x1, x1, #3
    str x1, [x0]

    adr x0, pud_ttbr0
    adr x1, pmd_ttbr0
    orr x1, x1, #3
    str x1, [x0]

    adr x1, pmd_ttbr0
    mov x0, #(1 << 10 | 1 << 2 | 1 << 0)
    str x0, [x1]
    ret


// each level page table have 2^9=512 entries , each entry size is 64bit=8byte. 8*512=4096
.balign 4096
// kernel space tables
pgd_ttbr1:
    .space 4096
pud_ttbr1:
    .space 4096
pmd_ttbr1:
    .space 4096
pmd2_ttbr1:
    .space 4096

// user space tables
pgd_ttbr0:
    .space 4096
pud_ttbr0:
    .space 4096
pmd_ttbr0:
    .space 4096



