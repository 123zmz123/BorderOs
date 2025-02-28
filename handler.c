#include <stdint.h>
#include "print.h"

void handler(uint64_t id, uint64_t esr, uint64_t elr) {
    switch (id)
    {
    case 1:
        printk("sync error at:%x, reason:%x\r\n",elr,esr);
        while(1){}
    
    default:
        printk("unknown error\r\n");
        while(1){}
    }
}