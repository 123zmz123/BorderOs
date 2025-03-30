#include "../lib/lib.h"

int main(void)
{
    int pid;

    pid = fork();
    if (pid == 0) {
        printf("This is the new process\r\n");
    }
    else {
        printf("This is the current process\r\n");
        waitu(pid);
    }
    
    return 0;
}