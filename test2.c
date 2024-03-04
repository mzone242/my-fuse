#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

int
main() {
    FILE *fp = NULL;
    fclose(fopen("mountdir/foobar", "w"));
    printf("Created file\n");
    return 0;
}