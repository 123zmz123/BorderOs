#include "uart.h"
#include "print.h"
#include "debug.h"
#include "lib.h"
#include "handler.h"
#include "memory.h"
#include "file.h"
#include "stddef.h"
#include "process.h"
void KMain(void)
{
    uart_init();
    printk("Hello world\r\n");
    printk("we are at EL:%u\r\n",get_el());
    init_memory();
    init_fs();
    void *p = kalloc();
    ASSERT(p!=NULL);
    if (load_file("TEST.BIN", (uint64_t)p) == 0) {
        printk("loaded file is:%s\r\n", p);
    }
    init_timer();
    init_interrupt_controller();
    init_process();
    enable_irq();
    while (1)
    {
        ;
    }
}