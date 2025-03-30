#include "../lib/lib.h"

int main(void)
{
    int fd;
    int size;
    char buffer[100];

    fd = open_file("TEST.BIN");
    if (fd == -1) {
        printf("open file failed\r\n");
    }
    else {
        size = get_file_size(fd);
        size = read_file(fd, buffer, size);
        buffer[size] = '\0';
        printf("%s\r\n", buffer);
        printf("Read %dbytes in total \r\n", (int64_t)size);
    }

    close_file(fd);
    
    return 0;
}