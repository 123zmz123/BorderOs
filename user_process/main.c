#include "../lib/lib.h"

int main(void)
{
    int fd;
    int size;

    fd = open_file("TEST.BIN");
    if (fd == -1) {
        printf("open file failed\r\n");
    }
    else {
        size = get_file_size(fd);
        printf("The size of the file is %d \r\n", (int64_t)size);
    }

    close_file(fd);
    
    return 0;
}