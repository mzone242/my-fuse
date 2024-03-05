#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

int
main() {
    int fd0 = open("mountdir/large", O_RDWR);
    char buf[4096];
    int i;
    for (i = 0; i < 25600; i++) {
        int nb0 = read(fd0, buf, 4096);
    }
    close(fd0);
    return 0;
}