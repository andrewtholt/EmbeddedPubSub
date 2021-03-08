CC=gcc

CFLAGS=-g
BINS=tst
HDR=Protocol/proto.c Dump/dump.c


tst:	tst.c $(HDR) Protocol/proto.c Dump/dump.c Makefile
	@make -C Dump
	@make -C Protocol
	$(CC) $(CFLAGS) tst.c Protocol/proto.o Dump/dump.o -o tst

clean:
	rm -f $(BINS) 
	make -C ./Protocol clean
	make -C ./Dump clean
