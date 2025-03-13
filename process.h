#ifndef _PROCESS_H
#define _PROCESS_H
#include "handler.h"
#include "file.h"
#include "lib.h"
struct Process {
    int pid;
    int state;
    // map to each process page table
    uint64_t page_map;
};

//we only support 10 process in our toy os
#define NUM_PROC 10
#define PROC_UNUSED 0
#define PROC_INIT 1
#define PROC_RUNNING 2

void init_process(void);


#endif