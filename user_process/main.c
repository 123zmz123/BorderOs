#include "../lib/lib.h"


int main(void)
{
    while (1) {
        printf("User process\r\n");
        sleepu(100);
    }

    return 0;
}