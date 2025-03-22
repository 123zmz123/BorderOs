#ifndef _HANDLER_H
#define _HANDLER_H
#include "stdint.h"

struct TrapFrame {
    int64_t x0;
    int64_t x1;
    //16*0
    int64_t x2;
    int64_t x3;
    //16*1
    int64_t x4;
    int64_t x5;
    //16*2
    int64_t x6;
    int64_t x7;
    //16*3
    int64_t x8;
    int64_t x9;
    //16*4
    int64_t x10;
    int64_t x11;
    //16*5
    int64_t x12;
    int64_t x13;
    //16*6
    int64_t x14;
    int64_t x15;
    //16*7
    int64_t x16;
    int64_t x17;
    //16*8
    int64_t x18;
    int64_t x19;
    //16*9
    int64_t x20;
    int64_t x21;
    //16*10
    int64_t x22;
    int64_t x23;
    //16*11
    int64_t x24;
    int64_t x25;
    //16*12
    int64_t x26;
    int64_t x27;
    //16*13
    int64_t x28;
    int64_t x29;
    //16*14
    int64_t x30;
    int64_t sp0;
    //16*15
    int64_t trapno;
    int64_t esr;
    //16*16
    int64_t elr;
    int64_t spsr;
    //16*17
};

void init_timer(void);
void enable_irq(void);
void init_interrupt_controller(void);
#endif