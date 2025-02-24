aarch64-linux-gnu-gcc -c boot.s -o boot.o
aarch64-linux-gnu-gcc -c lib.s -o lib.o
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only -c main.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only -c uart.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only -c print.c
aarch64-linux-gnu-gcc -std=c99 -ffreestanding -mgeneral-regs-only -c debug.c

aarch64-linux-gnu-ld -nostdlib -T link.lds -o kernel boot.o main.o lib.o uart.o print.o debug.o
aarch64-linux-gnu-objcopy -O binary kernel kernel.img