CC=gcc
CPP=g++

CPPFLAGS=-g -std=c++11
DEFS=-DLINUX -DMQ
BINS=tstMngmt tst


# all:	cmdParser.o Small.o
all:	$(BINS)

tst:	tst.cpp cmdParser.o linuxParser.o Small.o mngmt.o enums.h # comms.o
	$(CPP) $(CPPFLAGS) $(DEFS) cmdParser.o linuxParser.o Small.o mngmt.o tst.cpp -lrt -pthread -o tst

tstParser:	tstParser.cpp cmdParser.o linuxParser.o Small.o mngmt.o enums.h # comms.o
	$(CPP) $(CPPFLAGS) $(DEFS) tstParser.cpp cmdParser.o linuxParser.o Small.o mngmt.o -lrt -pthread -o tstParser

cmdParser.o:	cmdParser.cpp msgs.h
	$(CPP) $(CPPFLAGS) $(DEFS) -c cmdParser.cpp -o cmdParser.o

Small.o:	Small.cpp Small.h
	$(CPP) $(CPPFLAGS) $(DEFS) -c Small.cpp -o Small.o

comms.o: comms.h comms.cpp
	$(CPP) $(CPPFLAGS) $(DEFS) -c comms.cpp -o comms.o

mngmt.o: mngmt.cpp mngmt.h
	$(CPP) $(CPPFLAGS) $(DEFS) -c mngmt.cpp -o mngmt.o

tstMngmt:	mngmt.o tstMngmt.cpp
	$(CPP) $(CPPFLAGS) $(DEFS) tstMngmt.cpp mngmt.o -lrt -o tstMngmt

linuxParser.o:  linuxParser.cpp cmdParser.o cmdParser.h linuxParser.h
	$(CPP) $(CPPFLAGS) $(DEFS) -fpic -g -c linuxParser.cpp cmdParser.o -o linuxParser.o

clean:
	rm -f *.o cscope.out

