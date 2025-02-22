#include "uart.h"
#include "lib.h"

void uart_init(void){
    out_word(UART0_CR,0x0000);
    /* for config to baudrate 115200*/
    out_word(UART0_IBRD,26);
    /* for config to baudrate 115200 */
    out_word(UART0_FBRD,3); // or 0?

    /*for config parameter*/
    out_word(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    /*clear all uart0 interrupt mask*/
    out_word(UART0_IMSC,0);

    /*enable interrupt on fifo on*/
    out_word(UART0_CR,(1<<0) | (1<<8) | (1<<9));

}

void write_char(unsigned char c) {
    /*if transmit buff is full then while(1)*/
    while(in_word(UART0_FR) & (1 << 5))
    {
    };
    out_word(UART0_DR,c);
}

unsigned char read_char(){
    /*if read buffer is full, while(1)*/
    while (in_word(UART0_FR) & (1<<4))
    {
    }
    return in_word(UART0_DR);
}

void write_str(const char * str) {
    for (int i=0; str[i] != '\0';i++){
        write_char(str[i]);
    }
}