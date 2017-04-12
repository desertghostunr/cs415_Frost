# PA4: "Matrix Multiplication"

# Author 
Andrew Frost

# Dependencies, Building, and Running

## Dependency Instructions
These projects depend on the MPI Library and SLIURM Running environment preinstalled in [h1.cse.unr.edu](h1.cse.unr.edu).  There are no instructions included on how to install these suites on your personal machines.
If you are using a Windows machine, you may SSH into your respective UNR accounts through [PuTTY] (http://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html).

You may enter h1 off campus by first logging into your account through ubuntu.cse.unr.edu.  (Please note that keystrokes will be a tiny bit slower, but it's still manageable).
```bash
ssh username@ubuntu.cse.unr.edu
ssh username@h1.cse.unr.edu
```

## Building and Running
Build this project using the provided makefile.

# Quick running
The program must be build in the build directory.

You may build the project with a command.
```bash
cd build
make
```

All programs must be ran from the build directory.

All of the parameters to the scripts can be replaced with other values to test.
The parameters work as follows:

sbatch Sequential.sh [n: matrix dimensions] [save option: 1 to save, anything else to not save ]

sbatch Sequential.sh [mat A file] [mat B file] [save option: 1 to save, anything else to not save ]

To run the sequential algorithm use the following command:
```bash
sbatch Sequential.sh 3
```

or

```bash
sbatch Sequential.sh ../bin/matA.txt ../bin/matB.txt
```


To run a set of timing tests do the following:
```bash
chmod +x RunTests.sh
./RunTests.sh
```

### Makefile Instructions
The makefile works as expected. It must be ran inside of build.

```bash
cd build
make
```

To clean the files you've created, there is an extra target, `clean`.
```bash
make clean
```
