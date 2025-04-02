aarch64-linux-gnu-gcc -c start.s -o start.o
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only  -c main.c
aarch64-linux-gnu-ld -nostdlib -T link.lds -o console start.o main.o ../lib/lib.a
aarch64-linux-gnu-objcopy -O binary console console.bin