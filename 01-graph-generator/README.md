# Module 01: Graph Generator

This is the first in a series of modules that is part of a single large project. The project is to
help the local police department with their installation of security cameras at traffic intersections.
It will solve a particular kind of optimization problem, called the Vertex Cover problem, in this
context. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring.

More details [here](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/01-graph-generator/ece650.a1.pdf).

### Commands

+ `a` add a street
+ `c` change a street
+ `r` remove a street
+ `g` generate a graph (G = <V, E>)

### Sample Inputs and Outputs

**Input**

```
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)

a "King Street S" (4,2) (4,8)

a "Davenport Road" (1,4) (5,8)

g
```
**Output**

```
V = {

    1: (2,2)

    2: (4,2)

    3: (4,4)

    4: (5,5)

    5: (1,4)

    6: (4,7)

    7: (5,6)

    8: (5,8)

    9: (3,8)

    10: (4,8)

}

E = {

    <1,3>,

    <2,3>,

    <3,4>,

    <3,6>,

    <7,6>,

    <6,5>,

    <9,6>,

    <6,8>,

    <6,10>

}
```

### Error Inputs

1. Incorrect input format
2. Incorrect commands
3. `c` or `r` street that does not exist

**Recognize user input errors and give a brief descriptive message about the error, then continue to accept input**

### Termination
`Ctrl+D` will terminate the program immediately.
