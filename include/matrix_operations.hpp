#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

#include <cassert>

#include "matrix.hpp"
#include "vector.hpp"

namespace tws {

// Matrix-vector multiplication, store the result in a new vector
template <Matrix M, Vector V>
auto operator*(const M& A, const V& v)
{
    assert(A.num_columns() == v.size());
    vector<std::remove_const_t<typename M::val_t>> result(A.num_rows());
    for (int i = 0; i < A.num_rows(); ++i) {
        result[i] = 0;
        for (int j = 0; j < A.num_columns(); ++j) {
            result[i] += A(i, j) * v[j];
        }
    }
    return result;
}

// Matrix-vector multiplication, store the result in a given vector
template <Matrix M, Vector V1, Vector V2>
void multiply(const M& A, const V1& v, V2& result)
{
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
template <Matrix M1, Matrix M2>
auto operator*(const M1& A, const M2& B)
{
    assert(A.num_columns() == B.num_rows());
    matrix<std::remove_const_t<typename M1::val_t>> result(A.num_rows(),
                                                           B.num_columns());
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
template <Matrix M1, Matrix M2, Matrix M3>
void multiply(const M1& A, const M2& B, M3& result)
{
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
template <Matrix M>
auto transpose(const M& A)
{
    matrix<std::remove_const_t<typename M::val_t>> result(A.num_columns(),
                                                          A.num_rows());
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < A.num_columns(); ++j) {
            result(j, i) = A(i, j);
        }
    }
    return result;
}

// Transpose of a matrix, store the result in a given matrix
template <Matrix M1, Matrix M2>
void transpose(const M1& A, M2& result)
{
    assert(A.num_rows() == result.num_columns());
    assert(A.num_columns() == result.num_rows());
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < A.num_columns(); ++j) {
            result(j, i) = A(i, j);
        }
    }
}

// Frobenius norm of a matrix
template <Matrix M>
auto norm(const M& A)
{
    std::remove_const_t<typename M::val_t> result = 0;
    for (int i = 0; i < A.num_rows(); ++i) {
        for (int j = 0; j < A.num_columns(); ++j) {
            result += A(i, j) * A(i, j);
        }
    }
    return std::sqrt(result);
}

}  // namespace tws

#endif