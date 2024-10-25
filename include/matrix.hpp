
#ifndef matrix_hpp
#define matrix_hpp

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <random>

#include "vector.hpp"

namespace tws {

// This defines a matrix as an abstract concept.
// You can safely ignore this until you have learned about concepts.
template <typename M>
concept Matrix = requires(M m)
{
    {typename M::val_t()};
    {m.num_rows()};
    {m.num_columns()};
    {m.size()};
    {m(0, 0)};
};

// Forward declaration of the matrixview class
template <Scalar T = double>
class matrixview;

/**
 * A matrix class that can be used to store matrices of arbitrary size.
 *
 * The elements of the matrix are stored in column-major order
 *
 * @tparam T this is a template parameter that specifies the type of the
 *           elements of the vector.
 */
template <Scalar T = double>
class matrix {
    friend class matrixview<T>;

   public:
    typedef T val_t;

   public:
    /**
     * @brief Construct a new Matrix
     *
     * @param m  integer
     *           Number of rows of the matrix
     *
     * @param n  integer
     *           Number of columns of the matrix
     */
    matrix(int m, int n) : _m(m), _n(n), _data(new T[_m * _n])
    {
#ifndef NDEBUG
        if constexpr (std::is_floating_point<T>::value) {
            for (int j = 0; j < _n; ++j) {
                for (int i = 0; i < _m; ++i) {
                    _data[i + j * _m] = std::nan("1");
                }
            }
        }
#endif
    }

    // Destructor
    ~matrix()
    {
        // Nothing to do here, the shared pointer will take care of the memory
    }

    /**
     * @brief Construct a new Matrix object as a copy of another matrix
     *        This does a deep copy
     *
     * @param m Matrix
     *          Matrix to copy
     */
    matrix(const matrix& m)
        : _m(m.num_rows()),
          _n(m.num_columns()),
          _data(new T[m.num_rows() * m.num_columns()])
    {
        for (int j = 0; j < m.num_columns(); ++j) {
            for (int i = 0; i < m.num_rows(); ++i) {
                _data[i + j * _m] = m(i, j);
            }
        }
    }

    /**
     * @brief Construct a new Matrix object as a copy of another matrix
     *        This does a deep copy
     *
     * This is a templated version, you can safely ignore this until you
     * have learned about expression templates and more advanced C++ features.
     *
     * @param m Matrix
     *          Matrix to copy
     */
    template <Matrix M>
    matrix(const M& m)
        : _m(m.num_rows()),
          _n(m.num_columns()),
          _data(new T[m.num_rows() * m.num_columns()])
    {
        for (int j = 0; j < m.num_columns(); ++j) {
            for (int i = 0; i < m.num_rows(); ++i) {
                _data[i + j * _m] = m(i, j);
            }
        }
    }

    /**
     * @brief Construct a new Matrix object by moving the data of another matrix
     *        This does a shallow copy
     *
     * @param m Matrix
     *          Matrix to move
     */
    matrix(matrix&& m) : _m(m.num_rows()), _n(m.num_columns()), _data(m._data)
    {}

    /**
     * @brief Assign the data of another matrix to this matrix
     *        This does a deep copy
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    matrix& operator=(const matrix& m)
    {
        assert(m.num_rows() == _m);
        assert(m.num_columns() == _n);
        for (int i = 0; i < _m; ++i) {
            for (int j = 0; j < _n; ++j) {
                _data[i + j * _m] = m(i, j);
            }
        }
        return *this;
    }

    /**
     * @brief Assign the data of another matrix to this matrix
     *        This does a deep copy
     *
     * This is a templated version, you can safely ignore this until you
     * have learned about expression templates and more advanced C++ features.
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    template <Matrix M>
    matrix& operator=(const M& m)
    {
        assert(m.num_rows() == _m);
        assert(m.num_columns() == _n);
        for (int i = 0; i < _m; ++i) {
            for (int j = 0; j < _n; ++j) {
                _data[i + j * _m] = m(i, j);
            }
        }
        return *this;
    }

    /**
     * @brief Assign the data of another matrix to this matrix
     *        This does a shallow copy
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    matrix& operator=(matrix&& m)
    {
        assert(m.num_rows() == _m);
        assert(m.num_columns() == _n);
        _data = m._data;
        return *this;
    }

    // Returns the number of rows of the matrix
    inline int num_rows() const { return _m; }

    // Returns the number of columns of the matrix
    inline int num_columns() const { return _n; }

    // Returns the size of the matrix (i.e. the number of elements m * n)
    inline int size() const { return _n * _m; }

    // Returns the leading dimension of the matrix
    inline int ldim() const { return _m; }

    /**
     * @brief Access the ij-th element of the matrix
     *
     * @param i integer
     *          Row index
     * @param j integer
     *          Column index
     * @return  const T&
     *          Because this matrix is const, we return a const reference.
     */
    inline const T& operator()(int i, int j) const
    {
        assert(i < _m);
        assert(j < _n);
        return _data[i + j * _m];
    }

    /**
     * @brief Access the ij-th element of the matrix
     *
     * @param i integer
     *          Row index
     * @param j integer
     *          Column index
     * @return  T&
     */
    inline T& operator()(int i, int j)
    {
        assert(i < _m);
        assert(j < _n);
        return _data[i + j * _m];
    }

    /**
     * Return a submatrix of the matrix.
     *
     * @param i1 index of the first row of the submatrix
     * @param i2 index of the last row of the submatrix (not inclusive)
     * @param j1 index of the first column of the submatrix
     * @param j2 index of the last column of the submatrix (not inclusive)
     *
     * @example
     * matrix<float> A(5,5);
     * auto B = A.submatrix(1, 4, 1, 4); // B is a 3x3 submatrix of A
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline matrixview<T> submatrix(int i1, int i2, int j1, int j2) const
    {
        assert(i1 >= 0);
        assert(i2 <= _m);
        assert(j1 >= 0);
        assert(j2 <= _n);
        assert(i1 < i2);
        assert(j1 < j2);
        return matrixview<T>(i2 - i1, j2 - j1, _data, _m, i1 + j1 * _m);
    }

    /**
     * Return a column of matrix as a vector
     *
     * @param i index of the column
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline vectorview<T> column(int i) const
    {
        assert(i >= 0);
        assert(i < _n);
        return vectorview<T>(_m, &_data[i * _m], 1);
    }

    /**
     * Return a row of matrix as a vector
     *
     * @param i index of the row
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline vectorview<T> row(int i) const
    {
        assert(i >= 0);
        assert(i < _m);
        return vectorview<T>(_n, &_data[i], _m);
    }

    /**
     * Return a pointer to the data of the matrix.
     */
    inline T* data() { return _data.get(); }

    /**
     * Return a pointer to the data of the matrix.
     */
    inline const T* data() const { return _data.get(); }

   private:
    // Number of rows of the matrix
    const int _m;
    // Number of columns of the matrix
    const int _n;
    // Pointer to the data
    std::shared_ptr<T[]> _data;
};

/**
 * A matrix class that can be used to store matrices of arbitrary size.
 *
 * The elements of the matrix are stored in column-major order
 *
 * @tparam T this is a template parameter that specifies the type of the
 *           elements of the vector.
 */
template <Scalar T>
class matrixview {
   public:
    typedef T val_t;

   public:
    /**
     * @brief Construct a new Matrixview
     *
     * @param m      integer
     *               Number of rows of the matrix
     *
     * @param n      integer
     *               Number of columns of the matrix
     *
     * @param data   shared_ptr<T[]>
     *               Pointer to the data of the matrix
     *
     * @param ldim   integer
     *               Leading dimension of the matrix
     *               i.e. the number of elements between
     *               two elements of the same row.
     *
     * @param offset integer
     *               Offset of the matrix
     */
    matrixview(int m, int n, std::shared_ptr<T[]> data, int ldim, int offset)
        : _m(m), _n(n), _ldim(ldim), _offset(offset), _data(data)
    {
        assert(_ldim >= _m);
    }

    // Destructor
    ~matrixview()
    {
        // Nothing to do here, the shared pointer will take care of the memory
    }

    /**
     * @brief Construct a new matrixview object as a copy of another matrixview
     *        This does a shallow copy
     *
     * @param m matrixview
     *          Matrix to copy
     */
    matrixview(const matrixview& m)
        : _m(m.num_rows()),
          _n(m.num_columns()),
          _ldim(m.ldim()),
          _offset(m.offset()),
          _data(m._data)
    {}

    /**
     * @brief Construct a new matrixview object as a view of a matrix
     *
     * @param m matrix
     *          Matrix to view
     */
    matrixview(const matrix<T>& m)
        : _m(m.num_rows()),
          _n(m.num_columns()),
          _ldim(m.num_rows()),
          _offset(0),
          _data(m._data)
    {}

    /**
     * @brief Construct a new Matrix object by moving the data of another matrix
     *        This does a shallow copy
     *
     * @param m Matrix
     *          Matrix to move
     */
    matrixview(matrixview&& m)
        : _m(m.num_rows()),
          _n(m.num_columns()),
          _ldim(m.ldim()),
          _offset(m.offset()),
          _data(m._data)
    {}

    /**
     * @brief Assign the data of another matrix to this matrix
     *        This does a shallow copy
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    matrixview& operator=(const matrixview& m)
    {
        _m = m._m;
        _n = m._n;
        _ldim = m._ldim;
        _offset = m._offset;
        _data = m._data;
        return *this;
    }

    /**
     * @brief Assign the data of another matrix to this matrix
     *        This does a shallow copy
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    matrixview& operator=(matrixview&& m)
    {
        _m = m._m;
        _n = m._n;
        _ldim = m._ldim;
        _offset = m._offset;
        _data = m._data;
        return *this;
    }

    // Returns the number of rows of the matrix
    inline int num_rows() const { return _m; }

    // Returns the number of columns of the matrix
    inline int num_columns() const { return _n; }

    // Returns the leading dimension of the matrix
    inline int ldim() const { return _ldim; }

    // Returns the size of the matrix (i.e. the number of elements m * n)
    inline int size() const { return _n * _m; }

    // Returns the offset of the matrix
    // (The number of elements in the data before the first element of the
    // matrix)
    inline int offset() const { return _offset; }

    /**
     * @brief Access the ij-th element of the matrix
     *
     * @param i integer
     *          Row index
     * @param j integer
     *          Column index
     * @return  const T&
     *          Because this matrix is const, we return a const reference.
     */
    inline const T& operator()(int i, int j) const
    {
        assert(i < _m);
        assert(i >= 0);
        assert(j < _n);
        assert(j >= 0);
        return _data[_offset + i + j * _ldim];
    }

    /**
     * @brief Access the ij-th element of the matrix
     *
     * @param i integer
     *          Row index
     * @param j integer
     *          Column index
     * @return  T&
     */
    inline T& operator()(int i, int j)
    {
        assert(i < _m);
        assert(i >= 0);
        assert(j < _n);
        assert(j >= 0);
        return _data[_offset + i + j * _ldim];
    }

    /**
     * Return a submatrix of the matrix.
     *
     * @param i1 index of the first row of the submatrix
     * @param i2 index of the last row of the submatrix (not inclusive)
     * @param j1 index of the first column of the submatrix
     * @param j2 index of the last column of the submatrix (not inclusive)
     *
     * @example
     * matrix<float> A(5,5);
     * auto B = A.submatrix(1, 4, 1, 4); // B is a 3x3 submatrix of A
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline matrixview submatrix(int i1, int i2, int j1, int j2) const
    {
        assert(i1 >= 0);
        assert(i2 <= _m);
        assert(j1 >= 0);
        assert(j2 <= _n);
        assert(i1 < i2);
        assert(j1 < j2);
        return matrixview(i2 - i1, j2 - j1, _data, _ldim,
                          _offset + i1 + j1 * _ldim);
    }

    /**
     * Return a column of matrix as a vector
     *
     * @param i index of the column
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline vectorview<T> column(int i) const
    {
        assert(i >= 0);
        assert(i < _n);
        return vectorview<T>(_m, &_data[_offset + i * _ldim], 1);
    }

    /**
     * Return a row of matrix as a vector
     *
     * @param i index of the row
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline vectorview<T> row(int i) const
    {
        assert(i >= 0);
        assert(i < _m);
        return vectorview<T>(_n, &_data[_offset + i], _ldim);
    }

    /**
     * Return a pointer to the data of the matrix.
     */
    inline T* data() { return _data.get(); }

    /**
     * Return a pointer to the data of the matrix.
     */
    inline const T* data() const { return _data.get(); }

   private:
    // Number of rows of the matrix
    int _m;
    // Number of columns of the matrix
    int _n;
    // Leading dimension of the matrix
    int _ldim;
    // Offset of the matrix
    int _offset;
    // Pointer to the data
    std::shared_ptr<T[]> _data;
};

// Code for printing
template <Matrix M>
void print_matrix(const M& m)
{
    std::cout << "(" << m.num_rows() << "," << m.num_columns() << ")["
              << std::endl;
    for (int i = 0; i < m.num_rows(); ++i) {
        std::cout << "[";
        for (int j = 0; j < m.num_columns() - 1; ++j) {
            std::cout << m(i, j) << ",";
        }
        std::cout << m(i, m.num_columns() - 1) << "]" << std::endl;
    }
    std::cout << "]" << std::endl;
}

// Initialize a matrix with random values
template <Matrix M>
void randomize(M& m)
{
#ifdef NDEBUG
    std::random_device rd;
    std::mt19937 gen(rd());
#else
    // Note, when debugging, we want to have the same random numbers every time
    std::mt19937 gen(1302);
#endif

    typedef typename M::val_t T;

    if constexpr (std::is_integral<T>::value) {
        std::uniform_int_distribution<T> d(0, 100);
        for (int j = 0; j < m.num_columns(); ++j) {
            for (int i = 0; i < m.num_rows(); ++i) {
                m(i, j) = d(gen);
            }
        }
    }
    else {
        std::normal_distribution<T> d(0, 1);
        for (int j = 0; j < m.num_columns(); ++j) {
            for (int i = 0; i < m.num_rows(); ++i) {
                m(i, j) = d(gen);
            }
        }
    }
}

}  // namespace tws

#endif