# PA2: "Mandelbrot"

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
You may build script with a command.
```bash
make
```

To run on one box use the following command:
```bash
sbatch One_box.sh
```

To run on two boxes use the following command:
```bash
sbatch Two_box.sh
```
To run the timing test use the following command:
```bash
sbatch Timing.sh
```
### Makefile Instructions
The makefile works as expected.

```bash
make
```

To clean the files you've created, there is an extra target, `clean`.
```bash
make clean
```

### What are those command line parameters?
PA1 has the following command line parameters:

PA1 [ integer: number of ints ] [ integer: number of tests ] [ integer: number of timing tests to run ]

