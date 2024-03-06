#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

int
main() {
    FILE *fh;
    srand(time(NULL));
    fh = fopen("../mountdir/large", "r");
    if (fh == NULL)
    {
        perror("Unable to read file\n");
        return 1;
    }
    char buf[4096];
    int i;
    for (i = 0; i < 25600; i++) {
        int r = rand() % 25000;
        fseek(fh, r*sizeof(buf), SEEK_SET);
        int nb0 = fread(buf, sizeof(buf), 1, fh);
    }
    fclose(fh);
    return 0;
}