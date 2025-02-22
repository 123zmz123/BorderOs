#ifndef _LIB_H
#define _LIB_H

#include "stdint.h"

void delay(uint64_t value); // first parameter store in x0 reg when call
void out_word(uint64_t addr, uint32_t val); // para1 x1, para2 w1(half of x1)
uint32_t in_word(uint64_t addr);  //

#endif