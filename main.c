#include "uart.h"
#include "print.h"
#include "debug.h"
#include "lib.h"
#include "handler.h"
#include "memory.h"
#include "file.h"
#include "stddef.h"
#include "process.h"
#include "syscall.h"

void KMain(void)
{
    uart_init();
    printk("Hello world\r\n");
    printk("we are at EL:%u\r\n",get_el());
    init_memory();
    init_fs();
    init_system_call();
    init_timer();
    init_interrupt_controller();
    init_process();
    enable_irq();
}