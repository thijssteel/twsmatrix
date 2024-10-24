#ifndef VECTOR_OPERATIONS_HPP
#define VECTOR_OPERATIONS_HPP

#include <cassert>

#include "vector.hpp"

namespace tws {

// Add two vectors and store the result in a new vector
template <typename T>
vector<T> operator+(const vector<T>& v1, const vector<T>& v2) {
    assert(v1.size() == v2.size());
    vector<T> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

// Add two vectors and store the result in the first vector
template <typename T>
void operator+=(vector<T>& v1, const vector<T>& v2) {
    assert(v1.size() == v2.size());
    for (int i = 0; i < v1.size(); ++i) {
        v1[i] += v2[i];
    }
}

// Add two vectors and store the result in a third vector
template <typename T>
void add(const vector<T>& v1, const vector<T>& v2, vector<T>& result) {
    assert(v1.size() == v2.size());
    assert(v1.size() == result.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] + v2[i];
    }
}

// Subtract two vectors and store the result in a new vector
template <typename T>
vector<T> operator-(const vector<T>& v1, const vector<T>& v2) {
    assert(v1.size() == v2.size());
    vector<T> result(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

// Subtract two vectors and store the result in the first vector
template <typename T>
void operator-=(vector<T>& v1, const vector<T>& v2) {
    assert(v1.size() == v2.size());
    for (int i = 0; i < v1.size(); ++i) {
        v1[i] -= v2[i];
    }
}

// Subtract two vectors and store the result in a third vector
template <typename T>
void sub(const vector<T>& v1, const vector<T>& v2, vector<T>& result) {
    assert(v1.size() == v2.size());
    assert(v1.size() == result.size());
    for (int i = 0; i < v1.size(); ++i) {
        result[i] = v1[i] - v2[i];
    }
}

// Multiply a vector by a scalar and store the result in a new vector
template <typename T>
vector<T> operator*(const vector<T>& v, T scalar) {
    vector<T> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

// Multiply a vector by a scalar and store the result in a new vector
template <typename T>
vector<T> operator*(T scalar, const vector<T>& v) {
    vector<T> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

// Multiply a vector by a scalar and store the result in the vector
template <typename T>
void operator*=(vector<T>& v, T scalar) {
    for (int i = 0; i < v.size(); ++i) {
        v[i] *= scalar;
    }
}

// Multiply a vector by a scalar and store the result in a given vector
template <typename T>
void multiply(const vector<T>& v, T scalar, vector<T>& result) {
    assert(v.size() == result.size());
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
}

// Dot product of two vectors
template <typename T>
T dot(const vector<T>& v1, const vector<T>& v2) {
    assert(v1.size() == v2.size());
    T result = 0;
    for (int i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

// 2-Norm of a vector
template <typename T>
T norm(const vector<T>& v) {
    return std::sqrt(dot(v, v));
}

} // namespace tws

#endif