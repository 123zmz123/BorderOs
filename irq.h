#ifndef _IRQ_H
#define _IRQ_H
#include "memory.h"

#define CNTP_EL0	        P2V(0x40000040)
#define CNTP_STATUS_EL0     P2V(0x40000060)

#define BASE_ADDR           P2V(0x3f000000)

#define IRQ_BASIC_PENDING	(BASE_ADDR + 0xB200)
#define ENABLE_IRQS_1		(BASE_ADDR + 0xB210)
#define ENABLE_IRQS_2		(BASE_ADDR + 0xB214)
#define ENABLE_BASIC_IRQS	(BASE_ADDR + 0xB218)
#define DISABLE_IRQS_1		(BASE_ADDR + 0xB21C)
#define DISABLE_IRQS_2		(BASE_ADDR + 0xB220)
#define DISABLE_BASIC_IRQS	(BASE_ADDR + 0xB224)
 
#endif