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
	gcc -o tests/foo tests/foo.c

tests:
	gcc -o tests/large_read tests/large_read.c
	gcc -o tests/large_write tests/large_write.c
	gcc -o tests/large_read_random tests/large_read_random.c
	gcc -o tests/large_write_random tests/large_write_random.c

clean_tests:
	rm tests/large_read
	rm tests/large_write
	rm tests/large_read_random
	rm tests/large_write_random