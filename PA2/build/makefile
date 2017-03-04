CC=mpic++
CXXFLAGS= -O2 -Wall
LIBS=-lpmi

SRCDIR = ../src
S_OBJ_FILES= Mandelbrot.o PA2_Sequential.o PIMFuncs.o
P_S_OBJ_FILES= Mandelbrot.o PA2_Parallel_Static.o PIMFuncs.o
P_D_OBJ_FILES= Mandelbrot.o PA2_Parallel_Dynamic.o PIMFuncs.o

ifeq "$(wildcard $(SRCDIR) )" ""
	SRCDIR = src
endif

all: S PD PS

S: $(S_OBJ_FILES)
	$(CC) $(CXXFLAGS) -o PA2_Sequential $(S_OBJ_FILES) #$(LIBS)

PS: $(P_S_OBJ_FILES)
	$(CC) $(CXXFLAGS) -o PA2_Parallel_Static $(P_S_OBJ_FILES) $(LIBS)

PD: $(P_D_OBJ_FILES)
	$(CC) $(CXXFLAGS) -o PA2_Parallel_Dynamic $(P_D_OBJ_FILES) $(LIBS)

PA2_Sequential.o: $(SRCDIR)/PA2_Sequential.cpp $(SRCDIR)/Mandelbrot.h $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.h $(SRCDIR)/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/PA2_Sequential.cpp $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.cpp

PA2_Parallel_Static.o: $(SRCDIR)/PA2_Parallel_Static.cpp $(SRCDIR)/Mandelbrot.h $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.h $(SRCDIR)/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/PA2_Parallel_Static.cpp $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.cpp

PA2_Parallel_Dynamic.o: $(SRCDIR)/PA2_Parallel_Dynamic.cpp $(SRCDIR)/Mandelbrot.h $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.h $(SRCDIR)/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/PA2_Parallel_Dynamic.cpp $(SRCDIR)/Mandelbrot.cpp $(SRCDIR)/PIMFuncs.cpp

Mandelbrot.o: $(SRCDIR)/Mandelbrot.h $(SRCDIR)/Mandelbrot.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/Mandelbrot.cpp

PIMFuncs.o: $(SRCDIR)/PIMFuncs.h $(SRCDIR)/PIMFuncs.cpp
	$(CC) $(CXXFLAGS) -c $(SRCDIR)/PIMFuncs.cpp

clean:
	\rm *.o PA2_Sequential PA2_Parallel_Dynamic PA2_Parallel_Static
