# TWSMATRIX

This is a small library that defines a matrix and a vector class for use in the scientific software course at KU Leuven.

The implementation itself is not very general, robust or efficient and is mostly intented for educational purposes. Please use a better implementation for real work.

## Installation

The matrix library is a header-only library, so it doesn't need a separate compilation/installation step. Simply include the main header file in your code:
```cpp
#include "twsmatrix.hpp"
```
and make sure that the compiler knows where to find the header files. We recommend the following workflow:
- Clone this repository to your local machine.
- Create a new environment variable called `MATRIXLIBRARY` and set it to the directory where the header files are located. Note, this is not just the directory where you cloned this repository but the `include/` directory within it. To avoid having to recreate this variable every time you open a new terminal, you can add the following line to your `.bashrc` or `.bash_profile` file: `export MATRIXLIBRARY=/path/to/header/files`.
- When you compile your code, add the following flag to the compiler command:`-I$(MATRIXLIBRARY)`. This tells the compiler to look for header files in the directory specified by the `MATRIXLIBRARY` environment variable.

## Usage

### Creating a vector or matrix

The library is designed to make it easy to create matrices and vectors. Start by including the header file:
```cpp
#include "twsmatrix.hpp"
```
You can create a vector of length 10 with the following code:
```cpp
tws::vector<> v(10);
```
Similarly, you can create a matrix of size 10x15 with the following code:
```cpp
tws::matrix<> m(10, 15);
```
By default, matrices and vectors contain double precision floating point numbers. You can change the type by providing a template argument between the brackets. For example, to create a matrix of integers, you can use the following code:
```cpp
tws::matrix<int> m(10, 15);
```

It is important to remember that creating a matrix or vector allocates memory. While this is usually a negligible cost, it can be a problem if you create many small matrices or vectors in a loop.

### Accessing elements


You can access elements of a vector using the `[]` operator. For example, to set the first element of a vector to 5, you can use the following code:
```cpp
v[0] = 5;
```

You can access elements of a matrix using the `()` operator. For example, to set the element in the second row and third column of a matrix to 5, you can use the following code:
```cpp
m(1, 2) = 5;
```

If a matrix or vector is declared as `const`, you can only read elements, not write to them. For example, the following code will not compile:
```cpp
const tws::Vector<> v(10);
v[0] = 5; // This will not compile
```

### Submatrices and subvectors

You can create submatrices and subvectors using the `submatrix` and `subvector` functions. For example, to create a submatrix of the first 3 rows and 4 columns of a matrix `m`, you can use the following code:
```cpp
tws::matrix<> sub = m.submatrix(0, 3, 0, 4);
```
The `sub` matrix will be a view into the original matrix `m`, so changing elements in `sub` will also change the corresponding elements in `m`. If you don't want this behavior, you can create a copy of the submatrix:
```cpp
    tws::matrix<> sub(3,4);
    sub = m.submatrix(0, 3, 0, 4);
```
The same applies to subvectors.

You can also select a row or a column of a matrix using the `row` and `colomn` functions. For example, to select the second row of a matrix `m`, you can use the following code:
```cpp
tws::vector<> row = m.row(1);
```
To select the third column of a matrix `m`, you can use the following code:
```cpp
tws::vector<> column = m.column(2);
```
Just like a submatrix or subvector, a row or a column is a view into the original matrix, so changing elements in the row or column will also change the corresponding elements in the original matrix.


### Useful operations

There are some useful operations that you can perform on matrices and vectors. For example, you can transpose a matrix using the `transpose` function:
```cpp
tws::matrix<> mt = m.transpose();
```
You can also multiply two matrices using the `operator*`:
```cpp
tws::matrix<> result = m1 * m2;
```
These functions will automatically check if the dimensions of the matrices are compatible for the operation.\\
\newline
They will also create a new matrix to store the results. As mentioned before, this can be expensive if you are doing many operations in a loop. To avoid performance issues, you can use the alternative function `multiply`, which multiplies two matrices and stores the result in a pre-allocated matrix:
```cpp
tws::matrix<> result(10, 15);
multiply(m1, m2, result);
```
Check the `matrix_operations.hpp` and `vector_operations.hpp` files for more operations that you can perform on matrices and vectors.
