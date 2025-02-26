#ifndef _LIB_H
#define _LIB_H

#include "stdint.h"

void delay(uint64_t value); // first parameter store in x0 reg when call
void out_word(uint64_t addr, uint32_t val); // para1 x1, para2 w1(half of x1)
uint32_t in_word(uint64_t addr);  // read memory content 

/*set memory content to parameter2->value*/
void memset(void *dst, int value, unsigned int size);
/*copy from src to dst*/
void memcpy(void *dst, void *src, unsigned int size);
void memmov(void *dst, void *src, unsigned int size);

/*compare memory content between src1 and src2*/
int memcmp(void *src1, void *src2, unsigned int size);

/*gel exception level from system reg currenel */
uint8_t get_el(void);

#endif