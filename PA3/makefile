CC=mpic++ -std=c++0x
CXXFLAGS= -Wall #-O2 -Wall
LIBS=-lpmi

SRCDIR = ../src
S_OBJ_FILES= Sequential.o Timer.o
P_OBJ_FILES= Parallel.o Timer.o

ifeq "$(wildcard $(SRCDIR) )" ""
	SRCDIR = src
endif

all: S G

G: generator.o
	mpicc -o generator generator.o

S: $(S_OBJ_FILES)
	$(CC) $(CXXFLAGS) -o Sequential $(S_OBJ_FILES)

P: $(P_S_OBJ_FILES)
	$(CC) $(CXXFLAGS) -o Parallel $(P_OBJ_FILES) $(LIBS)

generator.o: $(SRCDIR)/generator.c
	mpicc -c $(SRCDIR)/generator.c 

Sequential.o: $(SRCDIR)/Sequential.cpp $(SRCDIR)/tSort.h $(SRCDIR)/Timer.h $(SRCDIR)/Timer.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/Sequential.cpp $(SRCDIR)/Timer.cpp

Parallel.o: $(SRCDIR)/Parallel.cpp $(SRCDIR)/tSort.h $(SRCDIR)/Timer.h $(SRCDIR)/Timer.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/Sequential.cpp $(SRCDIR)/Timer.cpp

Timer.o: $(SRCDIR)/Timer.h $(SRCDIR)/Timer.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/Timer.cpp

clean:
	\rm *.o Sequential generator #Parallel