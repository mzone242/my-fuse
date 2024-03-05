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
#include <stdlib.h>
#include <time.h>


const char *rootdir = "/tmp/matthewh/";
const char *rootdir_server = "matthewh@pegasus.cs.utexas.edu:/tmp/matthewh/";

#define PATH_MAX        4096

static void myfs_fullpath(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, rootdir);
    strncat(fpath, path, PATH_MAX - strlen(fpath) - 1); // ridiculously long paths will
				    // break here
}

static void myfs_fullpath_server(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, rootdir_server);
    strncat(fpath, path, PATH_MAX- strlen(fpath) - 1); // ridiculously long paths will
				    // break here
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int st_size, st_blocks, st_blksize, st_dev, st_ino, st_nlink, st_mode, st_uid, st_gid;
    char fpath[PATH_MAX];
	char spath[PATH_MAX];
	char scp[PATH_MAX*3];

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else {
		char p[1024];
		snprintf(scp, PATH_MAX*2, "ssh -i ~/.ssh/id_rsa.pub matthewh@pegasus.cs.utexas.edu stat %s", fpath);
		FILE *fp = popen(scp, "r");
		if (fp == NULL)
			return -errno;

		if (fgets(p, sizeof(p), fp) == NULL) {
			pclose(fp);
			return -errno;
		}
		if (fgets(p, sizeof(p), fp) == NULL) {
			pclose(fp);
			return -errno;
		}
		sscanf(p, "%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_size, &st_blocks, &st_blksize);
		stbuf->st_size = st_size;
		stbuf->st_blocks = st_blocks;
		if (fgets(p, sizeof(p), fp) == NULL) {
			pclose(fp);
			return -errno;
		}
		sscanf(p, "%*[^/]%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d", &st_dev, &st_ino, &st_nlink);
		stbuf->st_nlink = st_nlink;
		if (fgets(p, sizeof(p), fp) == NULL) {
			pclose(fp);
			return -errno;
		}
		sscanf(p, "%*[^0123456789]%o%*[^0123456789]%d%*[^0123456789]%d", &st_mode, &st_uid, &st_gid);
		stbuf->st_mode = S_IFREG | st_mode;
		stbuf->st_uid = st_uid;
		stbuf->st_gid = st_gid;

		pclose(fp);
	}

	return 0;
}

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
	int res, err;
    char fpath[PATH_MAX];
	char spath[PATH_MAX];
    char scp[PATH_MAX*3];

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);


    snprintf(scp, PATH_MAX*3, "scp -i ~/.ssh/id_rsa.pub -p %s %s", spath, fpath);
    err = system(scp);
	if (err == -1)
		return -errno;

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;
	fi->fh = res;
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	res = pread(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int res;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);

	res = pwrite(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

static int xmp_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	int err;
    char fpath[PATH_MAX];
	char spath[PATH_MAX];
    char scp[PATH_MAX*3];

    myfs_fullpath(fpath, path);
    myfs_fullpath_server(spath, path);


    snprintf(scp, PATH_MAX*3, "scp -i ~/.ssh/id_rsa.pub -p %s %s", fpath, spath);
    err = system(scp);
	if (err == -1)
		return -errno;

	(void) isdatasync;
	(void) fi;
	return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi)
{
    int err, res;
    char fpath[PATH_MAX];

	err = xmp_fsync(path, 0, fi);
	if (err == -1)
		return -errno;
	
	res = close(fi->fh);
	if (res == -1)
		return -errno;

    myfs_fullpath(fpath, path);
	err = remove(fpath);
	if (err == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.truncate	= xmp_truncate,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
