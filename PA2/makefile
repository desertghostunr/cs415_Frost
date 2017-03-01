CC=g++ #mpic++
CXXFLAGS=-Wall
LIBS=-lpmi

SRCDIR = ../src
OBJ_FILES= Mandelbrot.o PA2_Sequential.o PIMFuncs.o

ifeq "$(wildcard $(SRCDIR) )" ""
	SRCDIR = src
endif

all: $(OBJ_FILES)
	$(CC) $(CXXFLAGS) -o PA2_Sequential $(OBJ_FILES) #$(LIBS)

PA2_Sequential.o: $(SRCDIR)/PA2_Sequential.cpp $(SRCDIR)/Mandelbrot.h $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.h $(SRCDIR)/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/PA2_Sequential.cpp $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.cpp

Mandelbrot.o: $(SRCDIR)/Mandelbrot.h $(SRCDIR)/Mandelbrot.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/Mandelbrot.cpp

PIMFuncs.o: $(SRCDIR)/PIMFuncs.h $(SRCDIR)/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/PIMFuncs.cpp

clean:
	\rm *.o PA2
