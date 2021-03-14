
BINS=tstDump
CFLAGS=-g
CC=gcc
CPP=g++

BINS=tstProto tstKVStore tstDump dbgTst

OBJ=proto.o dump.o kvs.o db.o # espDb.o

all:	$(OBJ)

tst:	$(BINS)

dump.o:	dump.c
	$(CC) -c -g dump.c -o dump.o

tstDump:	tstDump.c dump.o
	$(CC) $(CFLAGS) tstDump.c dump.o -o tstDump

tstProto:	tstProto.c proto.o dump.o Makefile kvs.o
	$(CC) $(CFLAGS) tstProto.c proto.o dump.o kvs.o -o tstProto

dbgTst:	dbgTst.c kvs.o
	$(CC) $(CFLAGS) -DDEBUG dbgTst.c kvs.o -o dbgTst

help:
	@echo "Help"
	@echo "make         Build object for linking"
	@echo "make tst     Build test program"
	@echo "make clean   Delete all binaries and build products."

proto.o:    proto.c proto.h
	$(CC) $(CFLAGS) -c proto.c -o proto.o


kvs.o:	kvs.c kvs.h
	$(CC) $(CFLAGS) -c kvs.c -o kvs.o

tstKVStore:	tstKVStore.c kvs.o
	$(CC) $(CFLAGS) tstKVStore.c kvs.o -o tstKVStore

db.o:	db.cpp db.h
	$(CPP) $(CFLAGS) -c db.cpp -o db.o

espDb.o:	espDb.cpp espDb.h
	$(CPP) $(CFLAGS) -c espDb.cpp -o espDb.o

dbTst:	dbTst.cpp espDb.o db.o kvs.o
	$(CPP) $(CFLAGS) dbTst.cpp kvs.o espDb.o  db.o -o dbTst

clean:
	rm -f *.o $(BINS) $(OBJS)



