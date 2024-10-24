#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

#include <cassert>

#include "matrix.hpp"
#include "vector.hpp"

namespace tws {

// Matrix-vector multiplication, store the result in a new vector
template <typename T>
vector<T> operator*(const matrix<T>& A, const vector<T>& v) {
    assert(A.num_columns() == v.size());
    vector<T> result(A.num_rows());
    for (int i = 0; i < A.num_rows(); ++i) {
        result[i] = 0;
        for (int j = 0; j < A.num_columns(); ++j) {
            result[i] += A(i, j) * v[j];
        }
    }
    return result;
}

// Matrix-vector multiplication, store the result in a given vector
template <typename T>
void multiply(const matrix<T>& A, const vector<T>& v, vector<T>& result) {
    assert(A.num_columns() == v.size());
    assert(A.num_rows() == result.size());
    for (int i = 0; i < A.num_rows(); ++i) {
        result[i] = 0;
        for (int j = 0; j < A.num_columns(); ++j) {
            result[i] += A(i, j) * v[j];
        }
    }
}

// Matrix-matrix multiplication, store the result in a new matrix
template <typename T>
matrix<T> operator*(const matrix<T>& A, const matrix<T>& B) {
    assert(A.num_columns() == B.num_rows());
    matrix<T> result(A.num_rows(), B.num_columns());
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < B.num_columns(); ++j) {
            result(i, j) = 0;
            for (int k = 0; k < A.num_columns(); ++k) {
                result(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return result;
}

// Matrix-matrix multiplication, store the result in a given matrix
template <typename T>
void multiply(const matrix<T>& A, const matrix<T>& B, matrix<T>& result) {
    assert(A.num_columns() == B.num_rows());
    assert(A.num_rows() == result.num_rows());
    assert(B.num_columns() == result.num_columns());
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < B.num_columns(); ++j) {
            result(i, j) = 0;
            for (int k = 0; k < A.num_columns(); ++k) {
                result(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

// Transpose of a matrix, store the result in a new matrix
template <typename T>
matrix<T> transpose(const matrix<T>& A) {
    matrix<T> result(A.num_columns(), A.num_rows());
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < A.num_columns(); ++j) {
            result(j, i) = A(i, j);
        }
    }
    return result;
}

// Transpose of a matrix, store the result in a given matrix
template <typename T>
void transpose(const matrix<T>& A, matrix<T>& result) {
    assert(A.num_rows() == result.num_columns());
    assert(A.num_columns() == result.num_rows());
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < A.num_columns(); ++j) {
            result(j, i) = A(i, j);
        }
    }
}

// Frobenius norm of a matrix
template <typename T>
T norm(const matrix<T>& A) {
    T result = 0;
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < A.num_columns(); ++j) {
            result += A(i, j) * A(i, j);
        }
    }
    return std::sqrt(result);
}


} // namespace tws

#endif