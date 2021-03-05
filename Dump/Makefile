
BINS=tstDump
CFLAGS=-g
CC=gcc


dump.o:	dump.c
	$(CC) -c -g dump.c -o dump.o

tst:	tstDump.c dump.o
	$(CC) $(CFLAGS) tstDump.c dump.o -o tst

help:
	@echo "Help"
	@echo "make         Build object for linking"
	@echo "make tst     Build test program"
	@echo "make clean   Delete all binaries and build products."


clean:
	rm -f *.o $(BINS)

