#ifndef _FILE_H
#define _FILE_H
#include "memory.h"
#define FS_BASE P2V(0x30000000)

void init_fs(void);
#endif