.global delay
.global out_word
.global in_word
.global memset
.global memcpy
.global memmov
.global memcmp

delay:
    subs x0, x0, #1 // x0 = x0-1
    bne delay // if x0!=0;goto delay
    ret

out_word:
    str w1, [x0] // store w1 to memory which address defined in x0
    ret

in_word:
    ldr w0, [x0] // load memmory(address=x0) to w0
    ret

/*void memset(void *dst, int value, unsigned int size);*/
memset:
    /*if size == 0*/
    cmp x2, #0
    beq memset_end

set:
    /*store value to dst,and dst++*/
    strb w1, [x0], #1
    /*size-=1,result update to Z flag*/
    subs x2, x2, #1
    /*size!=0 goto set, chck Z flag*/
    bne set

memset_end:
    ret

/*int memcmp(void *src1, void *src2, unsigned int size);*/
memcmp:
    mov x3, x0
    mov x0, #0

compare:
    /*check size==0?*/
    cmp x2, #0
    beq memcmp_end

    /*copy memory[src] to w_,src++*/
    ldrb w4, [x3], #1
    ldrb w5, [x1], #1
    /*size-=1*/
    sub x2, x2, #1
    cmp w4, w5
    beq compare
    /*not equal return 1*/
    mov x0, #1

memcmp_end:
    ret

/*void memcpy(void *dst, void *src, unsigned int size);*/
memmov:
memcpy:
    cmp x2, #0
    beq memcpy_end

    mov x4, #1

    cmp x1, x0
    /*if src >= dst, next step dst++ = src++*/
    bhs copy
    add x3, x1, x2
    cmp x3, x0
    /*if src+size < dst, next step dst++ = src++*/
    bls copy

overlap:
    sub x3, x2, #1
    add x0, x0, x3
    add x1, x1, x3
    neg x4, x4

copy:
    /* *dst=*src */
    ldrb w3, [x1]
    strb w3, [x0]
    add x0, x0, x4
    add x1, x1, x4

    subs x2, x2, #1
    bne copy


memcpy_end:
    ret