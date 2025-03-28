
#include "syscall.h"
#include "print.h"
#include "debug.h"
#include "stddef.h"
#include "process.h"
#include "handler.h"
static SYSTEMCALL system_calls[10];
static int sys_write(int64_t *argptr)
{
    // argptr[0]->[x1], argptr[1]->[x1+1]
    write_console((char*)argptr[0], (int)argptr[1]);
    return (int)argptr[1];
}
static int sys_sleep(int64_t *argptr)
{
    uint64_t ticks;
    uint64_t old_ticks;
    uint64_t sleep_ticks = argptr[0];

    ticks = get_ticks();
    old_ticks = ticks;

    while (ticks - old_ticks < sleep_ticks) {
        sleep(-1);
        ticks = get_ticks();
    }

    return 0;
}
void system_call(struct TrapFrame *tf) 
{   
    // x8->system_call_num
    int64_t i = tf->x8;
    int64_t param_count = tf->x0;
    int64_t *argptr = (int64_t*)tf->x1;

    if (param_count < 0 || i < 0 || i > 1) {
        // return -1 as error
        tf->x0 = -1;
        return;
    }

    tf->x0 = system_calls[i](argptr);
}
void init_system_call(void)
{
    system_calls[0] = sys_write;
    system_calls[1] = sys_sleep;
}
