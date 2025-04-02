aarch64-linux-gnu-gcc -c syscall.s -o syscall.o
aarch64-linux-gnu-gcc -c lib.s -o lib.o
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c print.c
aarch64-linux-gnu-ar rcs lib.a print.o syscall.o lib.o