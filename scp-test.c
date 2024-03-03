#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    execl("/usr/bin/scp", "scp", "hello.txt", "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/hello.txt", (char *)0);
    return 0;
}