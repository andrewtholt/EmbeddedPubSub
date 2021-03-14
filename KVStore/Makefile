
CC=gcc
CPP=g++

CFLAGS=-g

BINS=tstKVStore dbTst

kvs.o:	kvs.c kvs.h
	$(CC) $(CFLAGS) -c kvs.c -o kvs.o

all:	$(BINS)

tstKVStore:	tstKVStore.c kvs.o
	$(CC) $(CFLAGS) tstKVStore.c kvs.o -o tstKVStore

db.o:	db.cpp db.h
	$(CPP) $(CFLAGS) -c db.cpp -o db.o

espDb.o:	espDb.cpp espDb.h
	$(CPP) $(CFLAGS) -c espDb.cpp -o espDb.o

dbTst:	dbTst.cpp espDb.o db.o
	$(CPP) $(CFLAGS) dbTst.cpp kvs.o db.o espDb.o -o dbTst

clean:
	rm -f *.o $(BINS)
