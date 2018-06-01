# Splines Library
A multi-dimensional natural cubic spline library written in C++.

## Abstract
A spline is a continuous and differentiable piecewise function composed of cubic polynomials that intersect a set of given control points. They are commonly used in robotics and game development. This library can handle splines of any dimension for individual splines shall be calculated per dimension anyways.

## Building the source code
Spline is a library written in CodeBlocks and compiled with CMake. Source build steps are as follow:
1. Change directory to the root source folder of the library.
2. Make a new directory called "Release" and change to that directory.
3. Run `cmake -DCMAKE_BUILD_TYPE=Release ..`
4. Run `make`
5. The static library archive file (.a file) will be in the "lib" folder while a demonstration will be in "bin."

## Running demo
![Alt text](https://github.com/anotherAlex154/splines/blob/master/demo.png)
* Left click on the canvas to add control points
* Right click on the canvas to clear control points
* Have fun with it (;

## Sources
* Splines: http://mathworld.wolfram.com/CubicSpline.html
* Thomas Algorithm: https://www.cfd-online.com/Wiki/Tridiagonal_matrix_algorithm_-_TDMA_(Thomas_algorithm)
