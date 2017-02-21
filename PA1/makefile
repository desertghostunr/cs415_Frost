# set flags and compiler, as well as file var
CC=mpicc
CXXFLAGS=-Wall
LIBS=-lpmi
# feel free to either add files here (for more compilcated src's), or create more vars, or just manually type them in. Up to you.
SRCDIR = ../src

ifneq "$(wildcard $(SRCDIR) )" ""
	FILE=../src/PA1.c
else
	FILE = src/PA1.c
endif

# uncomment below if/when you have a folder called include that holds potential header files
#INCLUDES=-I../include
OUTPUT=PA1

all: $(OUTPUT)

$(OUTPUT): $(FILE)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(FILE) $(LIBS)

clean:
	rm PA1
