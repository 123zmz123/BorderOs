#include "uart.h"
#include "lib.h"

void uart_init(void){
    out_word(UART0_CR,0x0000);
    /* for config to baudrate 115200*/
    out_word(UART0_IBRD,26);
    /* for config to baudrate 115200 */
    out_word(UART0_FBRD,3); // or 0?

    /*for config parameter*/
    out_word(UART0_LCRH,  (1 << 5) | (1 << 6));
    /*RX int on,*/
    out_word(UART0_IMSC,(1<<4));

    /*enable interrupt on fifo on*/
    out_word(UART0_CR,(1<<0) | (1<<8) | (1<<9));

}

void write_char(unsigned char c)
{
    /*if uart is busy*/
    while (in_word(UART0_FR) & (1 << 3))
    {
    };
    out_word(UART0_DR, c);
}

unsigned char read_char()
{
    return in_word(UART0_DR);
}

void write_str(const char * str) {
    for (int i=0; str[i] != '\0';i++){
        write_char(str[i]);
    }
}

void uart_handler(void)
{
    uint32_t status = in_word(UART0_MIS);
    // rx int
    if(status & (1<<4)) {
        char ch = read_char();
        if (ch =='\r'){
            write_str("\r\n");
        }
        else{
            write_char(ch);
        }
    }
    out_word(UART0_ICR,(1 <<4 ));
}