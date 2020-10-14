# Module 03: Module Integeration

This is the third in a series of modules that is part of a single large project. The project is to
help the local police department with their installation of security cameras at traffic intersections.
It will solve a particular kind of optimization problem, called the Vertex Cover problem, in this
context. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring.

More details [here](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/03-module-integeration/ece650.a3.pdf).

### Overview

+ Modify the output format of the Python script from [Module 01](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/01-graph-generator) to match the input format
of the C++ program from [Module 02](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/02-shortest-path).
+ Modify the C++ program from Module 02 to output the input graph on standard output.
+ Write a program in C++ to generate random input for the Python script.
+ Write a driver program in C++ that uses Inter-Process Communication (IPC) to link the
output of the random input generator to the input of the Python script, and the output of the
Python script to the input of the C++ program from Module 02.

### Build Guide

```
cd path/to/module
mkdir build && cd build && cmake ../ && make install
```


### Executables Guide

+ [rgen](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/03-module-integeration/rgen.cpp): Random input generator which generates random inputs of street specifications for the [Module 01](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/01-graph-generator).
+ [ece650-a1.py](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/03-module-integeration/ece650-a1.py): Street graph generator from [Module 01](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/01-graph-generator) which takes as input rgen's outputs and outputs the generated graph.
+ [ece650-a2](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/03-module-integeration/ece650-a2.cpp): Shortest path finder from [Module 02](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/02-shortest-path) which accpets `s` command and outputs the shortest path between its arguments.
+ [ece650-a3](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/03-module-integeration/ece650-a3.cpp): The driver that uses multi-process and IPC to integerate all three executables above. It has the overall control of the whole system.

### Random Input Generator

+ `-s k`  where `k` is an integer >= 2. The number of streets should be a random integer in [2, k].
If this option is not specified, you should use a default of k = 10; that is, the number of streets
should be a random integer in [2, 10].
+ `-n k`  where `k` is an integer >= 1. The number of line-segments in each street should be a
random integer in [1, k]. Default: k = 5.
+ `-l k`  where `k` is an integer >= 5. Your process should wait a random number `w` seconds,
where w is in [5, k] before generating the next (random) input. Default: k = 5.
+ `-c k`  where `k` is an integer >= 1. Your process should generate (x, y) coordinates such that
every `x` and `y` value is in the range [-k, k]. For example, if k = 15, all of your coordinate values
should be integers between -15 and 15. Default: k = 20.

**Executing Process**

1. Issue enough `r` commands to clear any existing street database.
2. Issue `a` commands to add new streets satisfying the specification.
3. Issue a `g` command.
4. Wait for specified number of seconds and repeat.

### Driver

The driver program has the overall control which has at least three programs that run concurrently:

1. The random generator [rgen](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/03-module-integeration/rgen.cpp).
2. The Python script from [Module 01](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/01-graph-generator)
3. The program from [Module 02](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/02-shortest-path) that takes as input a graph-specification and computes shortest paths. 

The driver program should `fork()` two processes and `exec()` two of those programs, and then `exec()` the
third (so it turns into a process that corresponds to the third program). It should set up all IPC
appropriately beforehand. It should send normal output to stdout, error output to stderr, and take input from stdin. The only input it takes are `s` commands, that ask for a shortest path between vertices.


### Sample inputs and outputs

**Input**

```
 ./ece650-a3 -s 5 -n 4 -l 5
```

**Output**

```
V 8
E {<0,2>,<0,3>,<0,4>,<1,3>,<4,7>,<5,2>,<5,6>}
```

**Input**

```
s 2 4
```
**Output**

```
2-0-4
```

### Termination

All the processes should terminate gracefully (and quietly) once it sees EOF at stdin.
