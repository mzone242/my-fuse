/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o myfs
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

static const char *myfs_str = "myfs World!\n";
static const char *myfs_path = "/foo";
char *rootdir = "/tmp/testing";

#define PATH_MAX        4096

static void myfs_fullpath(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, rootdir);
    strncat(fpath, path, PATH_MAX); // ridiculously long paths will
				    // break here
}

static int myfs_getattr(const char *path, struct stat *stbuf)
{
	int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
	// int res = 0;
	// char fpath[PATH_MAX];

	// myfs_fullpath(fpath, path);
	// res = lstat(fpath, stbuf);

	// // memset(stbuf, 0, sizeof(struct stat));
	// // if (strcmp(path, "/") == 0) {
	// // 	stbuf->st_mode = S_IFDIR | 0755;
	// // 	stbuf->st_nlink = 2;
	// // } else if (strcmp(path, myfs_path) == 0) {
	// // 	stbuf->st_mode = S_IFREG | 0444;
	// // 	stbuf->st_nlink = 1;
	// // 	stbuf->st_size = strlen(myfs_str);
	// // } else
	// // 	res = -ENOENT;

	// return res;
}

int myfs_mknod(const char *path, mode_t mode, dev_t dev)
{
    int retstat;
    char fpath[PATH_MAX];

    myfs_fullpath(fpath, path);
    
    // On Linux this could just be 'mknod(path, mode, dev)' but this
    // tries to be be more portable by honoring the quote in the Linux
    // mknod man page stating the only portable use of mknod() is to
    // make a fifo, but saying it should never actually be used for
    // that.
    if (S_ISREG(mode)) {
	retstat = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
	if (retstat >= 0)
	    retstat = close(retstat);
    } else
	if (S_ISFIFO(mode))
	    retstat = mkfifo(fpath, mode);
	else
	    retstat = mknod(fpath, mode, dev);
    
    return retstat;
}

static int myfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
    int retstat = 0;
    DIR *dp;
    struct dirent *de;
    
    // once again, no need for fullpath -- but note that I need to cast fi->fh
    dp = (DIR *) (uintptr_t) fi->fh;

    // Every directory contains at least two entries: . and ..  If my
    // first call to the system readdir() returns NULL I've got an
    // error; near as I can tell, that's the only condition under
    // which I can get an error from readdir()
    de = readdir(dp);
    if (de == 0) {
	retstat = -errno;
	return retstat;
    }

    // This will copy the entire directory into the buffer.  The loop exits
    // when either the system readdir() returns NULL, or filler()
    // returns something non-zero.  The first case just means I've
    // read the whole directory; the second means the buffer is full.
    do {
	if (filler(buf, de->d_name, NULL, 0) != 0) {
	    return -ENOMEM;
	}
    } while ((de = readdir(dp)) != NULL);
    
    
    return retstat;
	// (void) offset;
	// (void) fi;

	// if (strcmp(path, "/") != 0)
	// 	return -ENOENT;

	// filler(buf, ".", NULL, 0);
	// filler(buf, "..", NULL, 0);
	// filler(buf, myfs_path + 1, NULL, 0);

	// return 0;
}

static int myfs_open(const char *path, struct fuse_file_info *fi)
{
    int res = 0;
    int fd;
    char fpath[PATH_MAX];
    
    myfs_fullpath(fpath, path);
    
    // if the open call succeeds, my retstat is the file descriptor,
    // else it's -errno.  I'm making sure that in that case the saved
    // file descriptor is exactly -1.
    fd = open(fpath, fi->flags);
    if (fd < 0)
	res = -ENOENT;
	
    fi->fh = fd;
    
    return res;
	// if (strcmp(path, myfs_path) != 0)
	// 	return -ENOENT;

	// if ((fi->flags & 3) != O_RDONLY)
	// 	return -EACCES;

	// return 0;
}

static int myfs_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{

    return pread(fi->fh, buf, size, offset);

	// size_t len;
	// (void) fi;
	// if(strcmp(path, myfs_path) != 0)
	// 	return -ENOENT;

	// len = strlen(myfs_str);
	// if (offset < len) {
	// 	if (offset + size > len)
	// 		size = len - offset;
	// 	memcpy(buf, myfs_str + offset, size);
	// } else
	// 	size = 0;
	

	// return size;
}


int myfs_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{

    return pwrite(fi->fh, buf, size, offset);
}

int myfs_release(const char *path, struct fuse_file_info *fi)
{
    // We need to close the file.  Had we allocated any resources
    // (buffers etc) we'd need to free them here as well.
    return close(fi->fh);
}

/** Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 */
int myfs_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
    
    // some unix-like systems (notably freebsd) don't have a datasync call
#ifdef HAVE_FDATASYNC
    if (datasync)
	return fdatasync(fi->fh);
    else
#endif	
	return fsync(fi->fh);
}

static struct fuse_operations myfs_oper = {
	.getattr	= myfs_getattr,
	.readdir	= myfs_readdir,
	.mknod		= myfs_mknod,
	.open		= myfs_open,
	.read		= myfs_read,
	.write		= myfs_write,
	.fsync		= myfs_fsync,
	.release	= myfs_release,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &myfs_oper, NULL);
}
