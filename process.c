#include "process.h"
#include "memory.h"
#include "debug.h"
#include "stddef.h"

static struct Process process_table[NUM_PROC];
static uint8_t pid_num = 1u;

static struct Process* find_unuse_proc(void)
{
    struct Process *process = NULL;
    for (int i = 0; i < NUM_PROC; i++) {
        if (process_table[i].state == PROC_UNUSED) {
            process = &process_table[i];
            break;
        }
    }
    
    return process;
}

static void init_idle_proc(void) 
{

    struct Process *process;
    process = find_unuse_proc();
    ASSERT(process == &process_table[0]);
    process->state = PROC_RUNNING;
    process->pid = 0;
    process->page_map = P2V(read_pgd());
}

void init_process(void)
{
    init_idle_proc();
}