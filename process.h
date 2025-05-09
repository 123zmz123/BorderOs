#ifndef _PROCESS_H
#define _PROCESS_H

#include "handler.h"
#include "file.h"
#include "lib.h"

struct Process {
    struct List *next;
    int pid;
    int state;
    int wait;
    uint64_t context;
    // map to each process page table
    uint64_t page_map;
    // kernel_mode stack
    uint64_t stack;
    struct FileDesc *file[100];
    // trapframe when enter kernel
    struct TrapFrame *tf;
};

struct ProcessControl {
    struct Process *current_process;
    struct HeadList ready_list;
    struct HeadList wait_list;
    struct HeadList kill_list;
};

#define STACK_SIZE (2*1024*1024)
//we only support 10 process in our toy os
#define NUM_PROC 10
#define PROC_UNUSED 0
#define PROC_INIT 1
#define PROC_RUNNING 2
#define PROC_READY 3
#define PROC_SLEEP 4
#define PROC_KILLED 5

void init_process(void);
struct ProcessControl* get_ProcessControl(void);
void yield(void);
void swap(uint64_t *prev, uint64_t next);
void trap_return(void);
void sleep(int wait);
void wake_up(int wait);
void exit(void);
void wait(int pid);
int fork(void);
int exec(struct Process *process, char *name);

#endif