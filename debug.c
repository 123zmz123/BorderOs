#include "debug.h"
#include "print.h"

void error_check(char *file, uint64_t line){
    printk("Assertion Fail,[%s:%u]\r\n",file,line);
    while(1){}
}