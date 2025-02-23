#include "uart.h"
#include "print.h"
void KMain(void)
{
    uart_init();
    printk("Hello world\r\n");
    printk("I am number %d\r\n",42);
    printk("I am number %x\r\n",0x42);
    while (1)
    {
        ;
    }
}