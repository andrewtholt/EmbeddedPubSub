
BINS=tstDump
CFLAGS=-g -m32 -fPIC
CC=gcc
CPP=g++

BINS= databaseTst tstProto tstKVStore tstDump dbgTst tstProto

OBJ=msg.o pubSub.o proto.o dump.o kvs.o db.o # espDb.o

all:	$(OBJ)

tst:	$(BINS)

libem.so:	msg.o proto.o dump.o kvs.o pubSub.o db.o
#	gcc $(CFLAGS) -shared -o libem.so kvs.o msg.o proto.o dump.o pubSub.o db.o
	gcc $(CFLAGS) -shared -o libem.so kvs.o msg.o proto.o dump.o db.o

msg.o:	msg.c msg.h Makefile
	$(CC) $(CFLAGS) -DLINUX -c -g msg.c -o msg.o

dump.o:	dump.c
	$(CC) $(CFLAGS) -c -g dump.c -o dump.o

pubSub.o:	pubSub.c pubSub.h
	$(CC) $(CFLAGS) -c -g pubSub.c -o pubSub.o

tstDump:	tstDump.c dump.o
	$(CC) $(CFLAGS) tstDump.c dump.o -o tstDump

databaseTst:	databaseTst.c db.o kvs.o
	$(CC) $(CFLAGS) -DLINUX -DDEBUG databaseTst.c db.o kvs.o -lstdc++ -o databaseTst

# tstProto:	tstProto.c proto.o dump.o Makefile kvs.o pubSub.o Makefile
#	$(CC) $(CFLAGS) tstProto.c proto.o dump.o kvs.o pubSub.o -lrt -o tstProto
#
tstProto:	tstProto.c libem.so
	$(CC) $(CFLAGS) -L. -lem -lrt tstProto.c -o tstProto

dbgTst:	dbgTst.c kvs.o msg.o msg.h proto.h dump.h libem.so
	$(CC) $(CFLAGS) -DLINUX dbgTst.c -L. -lem -lpthread -lrt -lstdc++ -o dbgTst
#	$(CC) $(CFLAGS) -DLINUX -DDEBUG dbgTst.c kvs.o msg.o proto.o dump.o db.o kvs.h -lpthread -lrt -o dbgTst

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

fred:	tstKVStore.c libem.so
	$(CC) $(CFLAGS) tstKVStore.c -L. -lem -lrt -o fred

db.o:	db.cpp db.h
	$(CPP) $(CFLAGS) -c db.cpp -o db.o

espDb.o:	espDb.cpp espDb.h
	$(CPP) $(CFLAGS) -c espDb.cpp -o espDb.o

dbTst:	dbTst.cpp espDb.o db.o kvs.o
	$(CPP) $(CFLAGS) dbTst.cpp kvs.o espDb.o  db.o -o dbTst

clean:
	rm -f *.o $(BINS) $(OBJS) libem.so



