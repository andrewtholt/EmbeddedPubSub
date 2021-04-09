
BINS=tstDump
CFLAGS=-g -m32
CC=gcc
CPP=g++

BINS=tstProto tstKVStore tstDump dbgTst

OBJ=msg.o pubSub.o proto.o dump.o kvs.o db.o # espDb.o

all:	$(OBJ)

tst:	$(BINS)

msg.o:	msg.c msg.h Makefile
	$(CC) $(CFLAGS) -DLINUX -c -g msg.c -o msg.o

dump.o:	dump.c
	$(CC) $(CFLAGS) -c -g dump.c -o dump.o

pubSub.o:	pubSub.c pubSub.h
	$(CC) $(CFLAGS) -c -g pubSub.c -o pubSub.o

tstDump:	tstDump.c dump.o
	$(CC) $(CFLAGS) tstDump.c dump.o -o tstDump

tstProto:	tstProto.c proto.o dump.o Makefile kvs.o pubSub.o Makefile
	$(CC) $(CFLAGS) tstProto.c proto.o dump.o kvs.o pubSub.o -lrt -o tstProto

dbgTst:	dbgTst.c kvs.o msg.o
	$(CC) $(CFLAGS) -DLINUX -DDEBUG dbgTst.c kvs.o msg.o -lpthread -lrt -o dbgTst

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



