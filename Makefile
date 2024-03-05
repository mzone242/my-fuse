myfs:
	gcc -o myfs myfs.c `pkg-config fuse --cflags --libs`

clean:
	fusermount -u mountdir
	rm myfs

setup:
	mkdir /tmp/matthewh

mount:
	./myfs mountdir

foo:
	gcc -o foo test.c

scp:
	gcc -o scp scp-test.c