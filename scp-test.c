#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>


char *rootdir = "/tmp/matthewh/";
char *rootdir_server = "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/";

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
    const char *path = "/baz";
    char fpath[PATH_MAX];
	char spath[PATH_MAX];
    int status = 0;

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);

    // snprintf
    char ssh[PATH_MAX*3];

    myfs_fullpath(fpath, path);
	snprintf(ssh, PATH_MAX*3, "ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu touch %s", fpath);
	int err = system(ssh);
    // unsigned int mode = 420;
    // char ssh[PATH_MAX*3];
    // snprintf(ssh, PATH_MAX*3, "ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu chmod %o %s", mode, fpath);
    // int err = system(ssh);
    // const char *path2 = "/bar";
    // char npath[PATH_MAX];

    // myfs_fullpath(npath, path2);
    // char scp[PATH_MAX*3];
    // snprintf(scp, PATH_MAX*3, "ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu mv %s %s", fpath, npath);
    // int err = system(scp);

    // pid_t p = fork();
    // if(p < 0) {
    //     perror("fork fail");
    //     exit(1);
    // } else if (p == 0) {
	//     execl("/usr/bin/scp", "scp", "-i", "~/.ssh/id_rsa.pub", "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/foo", "/tmp/testing", (char *)0);
    // } else {
    //     wait(&status);
    // }

    // snprintf
    // char scp[PATH_MAX*3];
    // snprintf(scp, PATH_MAX*3, "scp -i ~/.ssh/id_rsa.pub -p %s %s", spath, fpath);
    // int err = system(scp);

    // struct stat buf = {0};
    // struct stat *stbuf = &buf;
	// // memset(buf, 0, sizeof(struct stat));
    // int st_size, st_blocks, st_blksize, st_dev, st_ino, st_nlink, st_mode, st_uid, st_gid;

    // int ret = stat("matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/foo", buf);

    // int err = system("/usr/bin/ssh", "ssh", "-i", "~/.ssh/id_rsa.pub", "matthewh@pegasus.cs.utexas.edu", "\'chmod 644 /tmp/testing/a\'", (char *)0);
    // char p[1035];
    // FILE *fp = popen("ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu stat /tmp/matthewh/foo", "r");
    // if (fp == NULL) {
    //     printf("Failed to run command\n");
    //     exit(1);
    // }

    // fgets(p, sizeof(p), fp);
    // printf("%s", p);
    // fgets(p, sizeof(p), fp);
    // printf("%s", p);
    // sscanf(p, "%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_size, &st_blocks, &st_blksize);
    // printf("%d %d %d\n", st_size, st_blocks, st_blksize);
    // buf.st_size = st_size;
    // buf.st_blocks = st_blocks;
    // buf.st_blksize = st_blksize;
    // fgets(p, sizeof(p), fp);
    // printf("%s", p);
    // sscanf(p, "%*[^/]%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_dev, &st_ino, &st_nlink);
    // printf("%d %d %d\n", st_dev, st_ino, st_nlink);
    // buf.st_dev = st_dev;
    // buf.st_ino = st_ino;
    // buf.st_nlink = st_nlink;
    // fgets(p, sizeof(p), fp);
    // printf("%s", p);
    // sscanf(p, "%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_mode, &st_uid, &st_gid);
    // printf("%d %d %d\n", st_mode, st_uid, st_gid);
    // buf.st_mode = st_mode;
    // buf.st_uid = st_uid;
    // buf.st_gid = st_gid;
    // char p[1024];
    // char scp[2048];
    // snprintf(scp, PATH_MAX*2, "ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu stat %s", fpath);
    // FILE *fp = popen(scp, "r");
    // if (fp == NULL)
        // return -errno;

    // if (fgets(p, sizeof(p), fp) == NULL) {
    //     pclose(fp);
    //     // return -errno;
    // }
    // if (fgets(p, sizeof(p), fp) == NULL) {
    //     pclose(fp);
    //     // return -errno;
    // }
    // sscanf(p, "%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_size, &st_blocks, &st_blksize);
    // stbuf->st_size = st_size;
    // stbuf->st_blocks = st_blocks;
    // stbuf->st_blksize = st_blksize;
    // if (fgets(p, sizeof(p), fp) == NULL) {
    //     pclose(fp);
    //     // return -errno;
    // }
    // sscanf(p, "%*[^/]%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_dev, &st_ino, &st_nlink);
    // stbuf->st_dev = st_dev;
    // stbuf->st_ino = st_ino;
    // stbuf->st_nlink = st_nlink;
    // if (fgets(p, sizeof(p), fp) == NULL) {
    //     pclose(fp);
    //     // return -errno;
    // }
    // sscanf(p, "%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_mode, &st_uid, &st_gid);
    // stbuf->st_mode = st_mode;
    // stbuf->st_uid = st_uid;
    // stbuf->st_gid = st_gid;
    // // pclose(fp);


    // while (fgets(p, sizeof(p), fp) != NULL) {
    //     printf("%s", p);
    // }

    // pclose(fp);


    printf("all good!\n");

    // execl("/usr/bin/scp", "scp", "hello.txt", "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/hello.txt", (char *)0);
    return 0;
}