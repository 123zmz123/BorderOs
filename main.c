#include "uart.h"
#include "print.h"
#include "debug.h"
#include "lib.h"
#include "handler.h"
void KMain(void)
{
    uart_init();
    printk("Hello world\r\n");
    printk("we are at EL:%u\r\n",get_el());
    init_timer();
    enable_irq();
    while (1)
    {
        ;
    }
}