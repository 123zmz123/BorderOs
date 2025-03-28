#include "process.h"
#include "memory.h"
#include "debug.h"
#include "stddef.h"

static struct Process process_table[NUM_PROC];
static uint8_t pid_num = 1u;
static struct ProcessControl pc;
void pstart(struct TrapFrame *tf);

struct ProcessControl* get_ProcessControl(void) 
{
    return &pc;
}
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
    struct ProcessControl *process_control;

    process = find_unuse_proc();
    ASSERT(process == &process_table[0]);

    process->state = PROC_RUNNING;
    process->pid = 0;
    process->page_map = P2V(read_pgd());

    process_control = get_ProcessControl();
    process_control->current_process = process;
}

static struct Process* alloc_new_process(void)
{
    struct Process *process;

    process = find_unuse_proc();
    ASSERT(process == &process_table[1]);

    // allocate one page for process stack
    process->stack = (uint64_t)kalloc();
    ASSERT(process->stack != 0);

    memset((void*)process->stack, 0, PAGE_SIZE);

    process->state = PROC_INIT;
    process->pid = pid_num++;

    process->context = process->stack + PAGE_SIZE - sizeof(struct TrapFrame) - 12*8;
    *(uint64_t*)(process->context + 11*8) = (uint64_t)trap_return;
    process->tf = (struct TrapFrame*)(process->stack + PAGE_SIZE - sizeof(struct TrapFrame));
    process->tf->elr = 0x400000;
    process->tf->sp0 = 0x400000 + PAGE_SIZE;
    process->tf->spsr = 0;

    // we still waste so much space oh my god!
    process->page_map = (uint64_t)kalloc();
    ASSERT(process->page_map != 0);
    memset((void*)process->page_map, 0, PAGE_SIZE);

    return process;
}

static void init_user_process(void)
{
    struct Process *process;
    struct ProcessControl *process_control;
    struct HeadList *list;

    process = alloc_new_process();
    ASSERT(process != NULL);

    ASSERT(setup_uvm((uint64_t)process->page_map, "INIT.BIN"));

    process_control = get_ProcessControl();
    list = &process_control->ready_list;

    process->state = PROC_READY;
    append_list_tail(list, (struct List*)process);
}


void init_process(void)
{
    init_idle_proc();
    init_user_process();
}

static void switch_process(struct Process *prev, struct Process *current) 
{
    switch_vm(current->page_map);
    // after swap we call trap_return
    swap(&prev->context, current->context);
}

// find a new process and execute
static void schedule(void) 
{
    struct Process *prev_proc;
    struct Process *current_proc;
    struct ProcessControl *process_control;
    struct HeadList *list;

    process_control = get_ProcessControl();
    list = &process_control->ready_list;
    prev_proc = process_control->current_process;

    if (is_list_empty(list)) {
        current_proc = &process_table[0];
    }
    else {
        current_proc = (struct Process*)remove_list_head(list);
    }

    current_proc->state = PROC_RUNNING;
    process_control->current_process = current_proc;

    // execute other process but start from kernel mode
    switch_process(prev_proc, current_proc);
}

void yield(void)
{
    struct Process *process;
    struct ProcessControl *process_control;
    struct HeadList *list;

    process_control = get_ProcessControl();
    list = &process_control->ready_list;

    if (is_list_empty(list)) {
        return;
    }

    process = process_control->current_process;
    process->state = PROC_READY;

    if (process->pid != 0) {
        append_list_tail(list, (struct List*)process);
    }

    schedule();
}

void sleep(int wait)
{
    struct Process *process;
    struct ProcessControl *process_control;

    process_control = get_ProcessControl();
    process = process_control->current_process;
    process->state = PROC_SLEEP;
    process->wait = wait;

    append_list_tail(&process_control->wait_list, (struct List*)process);
    schedule();
}

// if some process were wait we try add it to the ready list.then shed will restore it.
void wake_up(int wait)
{
    struct Process *process;
    struct ProcessControl *process_control;
    struct HeadList *ready_list;
    struct HeadList *wait_list;

    process_control = get_ProcessControl();
    ready_list = &process_control->ready_list;
    wait_list = &process_control->wait_list;

    process = (struct Process*)remove_list(wait_list, wait);

    while (process != NULL) {
        process->state = PROC_READY;
        append_list_tail(ready_list, (struct List*)process);
        process = (struct Process*)remove_list(wait_list, wait);
    }
}