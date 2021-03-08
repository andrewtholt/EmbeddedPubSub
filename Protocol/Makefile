CC=gcc

CFLAGS=-g
# BINS=tst

# tst:	tst.c proto.o Makefile
#	$(CC) $(CFLAGS) tst.c proto.o -o tst

proto.o:	proto.c proto.h
	$(CC) $(CFLAGS) -c proto.c -o proto.o


clean:
	rm -f $(BINS) *.o
