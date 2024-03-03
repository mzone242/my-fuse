#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>


char *rootdir = "/tmp/testing";
char *rootdir_server = "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh";

#define PATH_MAX        4096

static void myfs_fullpath(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, rootdir);
    strncat(fpath, path, PATH_MAX); // ridiculously long paths will
				    // break here
}

static void myfs_fullpath_server(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, rootdir_server);
    strncat(fpath, path, PATH_MAX); // ridiculously long paths will
				    // break here
}

int main(int argc, char const *argv[])
{
    const char *path = "/foo";
    char fpath[PATH_MAX];
	char spath[PATH_MAX];
    int status = 0;

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);

    // pid_t p = fork();
    // if(p < 0) {
    //     perror("fork fail");
    //     exit(1);
    // } else if (p == 0) {
	//     execl("/usr/bin/scp", "scp", "-i", "~/.ssh/id_rsa.pub", "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/foo", "/tmp/testing", (char *)0);
    // } else {
    //     wait(&status);
    // }
    int err = system("scp -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/foo /tmp/testing");
    // struct stat *buf;

    // int ret = stat("matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/foo", buf);

    // int err = system("/usr/bin/ssh", "ssh", "-i", "~/.ssh/id_rsa.pub", "matthewh@pegasus.cs.utexas.edu", "\'chmod 644 /tmp/testing/a\'", (char *)0);
    // char p[1035];
    // FILE *fp = popen("ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu stat /tmp/matthewh/a", "r");
    // if (fp == NULL) {
    //     printf("Failed to run command\n");
    //     exit(1);
    // }

    // while (fgets(p, sizeof(p), fp) != NULL) {
    //     printf("%s", p);
    // }

    // pclose(fp);


    printf("all good!\n");

    // execl("/usr/bin/scp", "scp", "hello.txt", "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/hello.txt", (char *)0);
    return 0;
}