#include <stdint.h>
#include "print.h"
#include "lib.h"
#include "irq.h"
#include "uart.h"
#include "handler.h"
#include "syscall.h"
#include "process.h"
/*defined in handler.s*/
void enable_timer(void);
uint32_t read_timer_status(void);
void set_timer_interval(uint32_t value);
uint32_t read_timer_freq(void);
/*end comment*/
static uint32_t timer_interval = 0u;
static uint64_t ticks = 0u;

void init_interrupt_controller(void)
{
    out_word(DISABLE_BASIC_IRQS, 0xffffffff);
    out_word(DISABLE_IRQS_1, 0xffffffff);
    out_word(DISABLE_IRQS_2, 0xffffffff);

    // BCM2837.pdf page113, uart_int <--> irq57, on irq2 bit25
    // enable uart_int
    out_word(ENABLE_IRQS_2, (1 << 25));
}

void init_timer(void)
{
   timer_interval = read_timer_freq() / 100; 
   enable_timer();
   // irq enable
   out_word(CNTP_EL0, (1 << 1));
}

static void timer_interrupt_handler(void)
{
    uint32_t status = read_timer_status();
    // timer irq is pending
    if (status & (1 << 2)) {
        ticks++;
        // try to wake up the process in wait_list.
        // and then call switch restore the process in kernel
        // mode
        wake_up(-1);
        set_timer_interval(timer_interval);
    }
}

static uint32_t get_irq_number(void)
{
    return in_word(IRQ_BASIC_PENDING);
}

uint64_t get_ticks(void)
{
    return ticks;
}
// tf where the address of sp
void handler(struct TrapFrame *tf) 
{
    uint32_t irq;
    int schedule = 0;
    switch (tf->trapno)
    {
    case 1:
        printk("sync error at:%x, reason:%x\r\n", tf->elr, tf->esr);
        while (1) { }
        break;

    case 2:
        irq = in_word(CNTP_STATUS_EL0);
        if (irq & (1 << 1)) {
            timer_interrupt_handler();
            schedule = 1;
        }
        else {
            irq = get_irq_number();
            if (irq & (1 << 19)) {
                uart_handler();
            }
            else {
                printk("unknown irq\r\n");
                while (1) { }
            }
        }
        break;

    // we calculate the syscall as 3
    case 3:
        system_call(tf);
        break;

    default:
        printk("unknown error\r\n");
        while (1) { }
    }

    if (schedule == 1) {
        yield();
    }
}