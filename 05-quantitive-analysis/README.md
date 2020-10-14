# Module 05: Quantitive Analysis

This is the fifth in a series of modules that is part of a single large project. The project is to
help the local police department with their installation of security cameras at traffic intersections.
It will solve a particular kind of optimization problem, called the Vertex Cover problem, in this
context. The idea is for the police to be able to minimize the number of cameras they need to
install, and still be as effective as possible with their monitoring.

More details [here](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/05-quantitive-analysis/ece650.project.pdf).

### Overview

+ Augment the code from [Module 04](https://github.com/Adiactive/Monitor-Location-Optimization/tree/master/04-vertex-cover) in the way that is described in [project description](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/05-quantitive-analysis/ece650.project.pdf).
+ Quantitatively analyze the software for various kinds of inputs.
+ Write a brief [report](https://github.com/Adiactive/Monitor-Location-Optimization/blob/master/05-quantitive-analysis/report.pdf) which includes all the analysis, results and graphs.

### Build Guide

```
cd path/to/module
git clone https://github.com/agurfinkel/minisat
mkdir build && cd build && cmake ../ && make install
```


