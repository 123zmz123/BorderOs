#include "uart.h"
void KMain(void)
{
    uart_init();
    write_str("Hello raspberry pi\r\n");
    while (1)
    {
        ;
    }
}