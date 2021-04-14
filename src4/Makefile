CC=gcc
CPP=g++

CPPFLAGS=-g -std=c++11
DEFS=-DLINUX -DMQ
BINS=tst


# all:	cmdParser.o Small.o
all:	$(BINS)

tst:	tst.cpp cmdParser.o Small.o
	$(CPP) $(CPPFLAGS) $(DEFS) tst.cpp cmdParser.o Small.o -lrt -o tst

cmdParser.o:	cmdParser.cpp msgs.h
	$(CPP) $(CPPFLAG) $(DEFS) -c cmdParser.cpp -o cmdParser.o

Small.o:	Small.cpp Small.h
	$(CPP) $(CPPFLAG) $(DEFS) -c Small.cpp -o Small.o

clean:
	rm -f *.o cscope.out

