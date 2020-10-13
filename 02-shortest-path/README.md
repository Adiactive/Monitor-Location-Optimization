# Module 02: Shortest Path

This is the second in a series of modules that is part of a single large project. The project is to
help the local police department with their installation of security cameras at traffic intersections.
It will solve a particular kind of optimization problem, called the Vertex Cover problem, in this
context. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring.

More details [here](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/02-shortest-path/ece650.a2.pdf).

### Overview

The program takes input from standard input, and outputs to standard output. It finds the shortest path between an arbitrary pair of vertex in a graph.

The graph is specified by the specification of the set of vertices `V` followed by the set of edges `E`,
in that order. `V` and `E` always occur together.

### Commands

+ `V` specify the number of vertex in a graph
+ `E` specify the set of edges
+ `s` find the shortest path between two vertex

### Sample inputs and outputs

**Input**

```
V 15
E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
s 2 10
```

**Output**

```
2-8-10
```

**Input**

```
V 5
E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
s 4 0
```
**Output**

```
4-1-2-0
```

### Error Inputs

1. Specify an edge whose origin or destination does not exist
2. Ask for a shortest path to a vertex that does not exist
3. Ask for a shortest path when
the vertices exist, but a path does not exist between them.

### Termination
`Ctrl+D` will terminate the program immediately.
