#ifndef VECTOR_OPERATIONS_HPP
#define VECTOR_OPERATIONS_HPP

#include <cassert>

#include "vector.hpp"

namespace tws {

// Add two vectors and store the result in a new vector
template <Vector V1, Vector V2>
auto operator+(const V1& v1, const V2& v2)
{
    assert(v1.size() == v2.size());
    vector<std::remove_const_t<typename V1::val_t>> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

// Add two vectors and store the result in the first vector
template <Vector V1, Vector V2>
void operator+=(V1& v1, const V2& v2)
{
    assert(v1.size() == v2.size());
    for (int i = 0; i < v1.size(); ++i) {
        v1[i] += v2[i];
    }
}

// Add two vectors and store the result in a third vector
template <Vector V1, Vector V2, Vector V3>
void add(const V1& v1, const V2& v2, V3& result)
{
    assert(v1.size() == v2.size());
    assert(v1.size() == result.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] + v2[i];
    }
}

// Subtract two vectors and store the result in a new vector
template <Vector V1, Vector V2>
auto operator-(const V1& v1, const V2& v2)
{
    assert(v1.size() == v2.size());
    vector<std::remove_const_t<typename V1::val_t>> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

// Subtract two vectors and store the result in the first vector
template <Vector V1, Vector V2>
void operator-=(V1& v1, const V2& v2)
{
    assert(v1.size() == v2.size());
    for (int i = 0; i < v1.size(); ++i) {
        v1[i] -= v2[i];
    }
}

// Subtract two vectors and store the result in a third vector
template <Vector V1, Vector V2, Vector V3>
void sub(const V1& v1, const V2& v2, V3& result)
{
    assert(v1.size() == v2.size());
    assert(v1.size() == result.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
}

// Multiply a vector by a scalar and store the result in a new vector
template <Vector V1, Scalar T>
vector<T> operator*(const V1& v, T scalar)
{
    vector<T> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

// Multiply a vector by a scalar and store the result in a new vector
template <Vector V1, Scalar T>
vector<T> operator*(T scalar, const V1& v)
{
    vector<T> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

// Multiply a vector by a scalar and store the result in the vector
template <Vector V1, Scalar T>
void operator*=(V1& v, T scalar)
{
    for (int i = 0; i < v.size(); ++i) {
        v[i] *= scalar;
    }
}

// Multiply a vector by a scalar and store the result in a given vector
template <Vector V1, Vector V2, Scalar T>
void multiply(const V1& v, T scalar, V2& result)
{
    assert(v.size() == result.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
}

// Dot product of two vectors
template <Vector V1, Vector V2>
auto dot(const V1& v1, const V2& v2)
{
    assert(v1.size() == v2.size());
    std::remove_const_t<typename V1::val_t> result = 0;
    for (int i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

// 2-Norm of a vector
template <Vector V1>
auto norm(const V1& v)
{
    return std::sqrt(dot(v, v));
}

}  // namespace tws

#endif