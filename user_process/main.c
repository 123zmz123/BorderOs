#include "../lib/lib.h"

int main(void)
{
    int fd;

    fd = open_file("TEST.BIN");
    if (fd == -1) {
        printf("open file failed\r\n");
    }
    else {
        printf("open file succeeds\r\n");
    }

    close_file(fd);
    
    return 0;
}