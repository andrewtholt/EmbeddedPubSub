
BINS=tstDump
CFLAGS=-g
CC=gcc

tstDump:	tstDump.c dump.o
	$(CC) $(CFLAGS) tstDump.c dump.o -o tstDump

dump.o:	dump.c
	$(CC) -c -g dump.c -o dump.o


clean:
	rm -f *.o $(BINS)

