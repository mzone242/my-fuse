foo:
	gcc -o foo.o foo.c

tests:
	gcc -o large_read.o large_read.c
	gcc -o large_write.o large_write.c
	gcc -o large_read_random.o large_read_random.c
	gcc -o large_write_random.o large_write_random.c

clean:
	rm *.o