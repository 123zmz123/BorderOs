#include "uart.h"
#include "print.h"
#include "debug.h"
#include "lib.h"
#include "handler.h"
#include "memory.h"
#include "file.h"
void KMain(void)
{
    uart_init();
    printk("Hello world\r\n");
    printk("we are at EL:%u\r\n",get_el());
    init_memory();
    init_fs();
    init_timer();
    init_interrupt_controller();
    // enable_irq();
    while (1)
    {
        ;
    }
}