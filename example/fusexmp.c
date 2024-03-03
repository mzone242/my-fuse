/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  Copyright (C) 2011       Sebastian Pipping <sebastian@pipping.org>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall fusexmp.c `pkg-config fuse --cflags --libs` -o fusexmp
*/

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>


char *rootdir = "/tmp/testing/";
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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res, status;
    char fpath[PATH_MAX];
	char spath[PATH_MAX];

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);

	if (access(fpath, F_OK) != 0) {
		pid_t p = fork();
		if(p < 0) {
			perror("fork fail");
			exit(1);
		} else if (p == 0) {
			execl("/usr/bin/scp", "scp", "-i", "~/.ssh/id_rsa.pub", spath, fpath, (char *)0);
		} else {
			wait(&status);
		}
	}


	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

// static int xmp_access(const char *path, int mask)
// {
// 	int res;

// 	res = access(path, mask);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

// static int xmp_readlink(const char *path, char *buf, size_t size)
// {
// 	int res;

// 	res = readlink(path, buf, size - 1);
// 	if (res == -1)
// 		return -errno;

// 	buf[res] = '\0';
// 	return 0;
// }


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

// static int xmp_mkdir(const char *path, mode_t mode)
// {
// 	int res;

// 	res = mkdir(path, mode);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

static int xmp_unlink(const char *path)
{
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

// static int xmp_rmdir(const char *path)
// {
// 	int res;

// 	res = rmdir(path);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

// static int xmp_symlink(const char *from, const char *to)
// {
// 	int res;

// 	res = symlink(from, to);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

// static int xmp_rename(const char *from, const char *to)
// {
// 	int res;

// 	res = rename(from, to);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

// static int xmp_link(const char *from, const char *to)
// {
// 	int res;

// 	res = link(from, to);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

// static int xmp_chown(const char *path, uid_t uid, gid_t gid)
// {
// 	int res;

// 	res = lchown(path, uid, gid);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res, status;
    char fpath[PATH_MAX];
	char spath[PATH_MAX];

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);


    pid_t p = fork();
    if(p < 0) {
        perror("fork fail");
        exit(1);
    } else if (p == 0) {
	    execl("/usr/bin/scp", "scp", "-i", "~/.ssh/id_rsa.pub", spath, fpath, (char *)0);
    } else {
        wait(&status);
    }

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

// static int xmp_statfs(const char *path, struct statvfs *stbuf)
// {
// 	int res;

// 	res = statvfs(path, stbuf);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */
    
	int status;
    char fpath[PATH_MAX];
	char spath[PATH_MAX];

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);


    pid_t p = fork();
    if(p < 0) {
        perror("fork fail");
        exit(1);
    } else if (p == 0) {
	    execl("/usr/bin/scp", "scp", "-i", "~/.ssh/id_rsa.pub", fpath, spath, (char *)0);
    } else {
        wait(&status);
    }

	(void) path;
	(void) fi;
	return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	// .access		= xmp_access,
	// .readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	// .mkdir		= xmp_mkdir,
	// .symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	// .rmdir		= xmp_rmdir,
	// .rename		= xmp_rename,
	// .link		= xmp_link,
	.chmod		= xmp_chmod,
	// .chown		= xmp_chown,
	.truncate	= xmp_truncate,
	
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	// .statfs		= xmp_statfs,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
