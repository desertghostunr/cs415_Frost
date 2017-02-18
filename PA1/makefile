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

# use this to run
run_same_box: $(OUTPUT)
	srun -n2 $(OUTPUT) 1 50

run_multiple_box: $(OUTPUT)
	srun -N2 $(OUTPUT) 1 50

run_timing_test: $(OUTPUT)
	srun -N2 $(OUTPUT) 100000 50 1000

clean:
	-@if rm $(OUTPUT) 2>/dev/null || true; then echo "Cleaned"; else echo "Nothing to clean"; fi
