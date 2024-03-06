myfs: setup
	gcc -o myfs myfs.c `pkg-config fuse --cflags --libs`

myfs_local_testing: setup
	gcc -o myfs myfs_local.c `pkg-config fuse --cflags --libs`

clean:
	fusermount -u mountdir
	rm myfs

setup:
	mkdir -p /tmp/matthewh

mount:
	./myfs mountdir