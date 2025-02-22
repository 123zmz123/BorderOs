.global delay
.global out_word
.global in_word
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
