#include "uart.h"
#include "print.h"
#include "debug.h"
#include "lib.h"
void KMain(void)
{
    uart_init();
    printk("Hello world\r\n");
    printk("we are at EL:%u\r\n",get_el());
    char *p = (char *)0xffff000000000000;
    *p = 1;

    printk("This message shouldn't be printed\r\n");
    while (1)
    {
        ;
    }
}