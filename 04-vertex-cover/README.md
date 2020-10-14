# Module 04: Vertex Cover

This is the fourth in a series of modules that is part of a single large project. The project is to
help the local police department with their installation of security cameras at traffic intersections.
It will solve a particular kind of optimization problem, called the Vertex Cover problem, in this
context. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring.

More details [here](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/04-vertex-cover/ece650.a4.pdf).

### Overview

Augment the code from [Module 02](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/02-shortest-path) to solve the minimal Vertex Cover problem for the input graph with [MiniSat](https://github.com/agurfinkel/minisat). 

This approach is based on a polynomial time reduction from Vertex Cover to CNF-SAT, and use of a SAT solver. More details regarding reduction and encoding can be seen [here](https://github.com/r24zeng/Street-cameras-installation/blob/master/04/a4_encoding.pdf) or in the quantitive analysis in [Module 05](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/05-quantitive-analysis)

### Build Guide

```
cd path/to/module
mkdir build && cd build && cmake ../ && make install
```

**Executing Process**

As soon as the program gets an input graph via the `V` and `E` specification it should compute a minimum-sized Vertex Cover, and immediately output it. The output is a sequence of vertices in increasing order separated by one space each.


### Sample inputs and outputs

**Input**

```
./ece650-a4
V 5
E {<0,4>,<4,1>,<0,3>,<3,4>,<3,2>,<1,3>}
```

**Output**

```
3 4
```

### Termination

All the processes should terminate gracefully (and quietly) once it sees EOF at stdin.
