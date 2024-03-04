myfs:
	gcc -o myfs myfs.c `pkg-config fuse --cflags --libs`
