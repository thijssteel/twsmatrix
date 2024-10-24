
#ifndef matrix_hpp
#define matrix_hpp

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>

#include "vector.hpp"

namespace tws {

// This defines a matrix as an abstract concept.
// You can safely ignore this untill you have learned about concepts.
#if(__cplusplus >= 202002L)

template <typename M>
concept Matrix = requires(M m)
{
    {typename M::val_t()};
    {m.num_rows()};
    {m.num_columns()};
    {m.size()};
    {m(0,0)};
};

#else

#define Matrix typename

#endif

/**
 * A matrix class that can be used to store matrices of arbitrary size.
 *
 * The elements of the matrix are stored in column-major order
 *
 * @tparam T this is a template parameter that specifies the type of the
 *           elements of the vector.
 */
template <typename T = double>
class matrix {
   public:
    typedef T val_t;
    typedef int idx_t;

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
    matrix(idx_t m, idx_t n)
        : m_(m),
          n_(n),
          ldim_(m),
          locally_allocated_(true),
          data_(new T[m_ * n_])
    {
        #ifndef NDEBUG
        std::fill(data_, data_ + m_ * n_, NAN);
        #endif
    }

    /**
     * @brief Construct a new Matrix without allocating memory
     *
     * @param m    integer
     *             Number of rows of the matrix
     * 
     * @param n    integer
     *             Number of columns of the matrix
     * 
     * @param ptr  pointer to the data
     *             Pointer to the data of the matrix
     *             Note: this constructor does not check whether the data
     *             pointed to by ptr is large enough to store the matrix.
     *             Use this constructor with caution.
     * 
     * @param ldim integer
     *             Leading dimension of the matrix
     */
    matrix(idx_t m, idx_t n, T* ptr, idx_t ldim)
        : m_(m), n_(n), ldim_(ldim), locally_allocated_(false), data_(ptr)
    {
        assert(ldim >= m);
    }

    // Destructor
    ~matrix()
    {
        if (locally_allocated_) {
            delete[] data_;
        }
    }

    /**
     * @brief Construct a new Matrix object as a copy of another matrix
     *
     * Note: this contructor makes a deep copy of the matrix and allocates
     * new memory to facilitate this.
     *
     * @param m Matrix
     *          Matrix to copy
     */
    matrix(const matrix& m)
        : m_(m.num_rows()),
          n_(m.num_columns()),
          ldim_(m.num_rows()),
          locally_allocated_(true),
          data_(new T[m.num_rows() * m.num_columns()])
    {
        for (int j = 0; j < m.num_columns(); ++j) {
            for (int i = 0; i < m.num_rows(); ++i) {
                data_[i + j * ldim_] = m(i, j);
            }
        }
    }

    /**
     * @brief Construct a new Matrix object by moving the data of another matrix
     *        Note, we do a deep copy and not a shallow copy
     *        even though the given matrix is temporary.
     *
     * @param m Matrix
     *          Matrix to move
     */
    matrix(matrix&& m)
        : m_(m.num_rows()),
          n_(m.num_columns()),
          ldim_(m.num_rows()),
          locally_allocated_(true),
          data_(new T[m.num_rows() * m.num_columns()])
    {
        for (int j = 0; j < m.num_columns(); ++j) {
            for (int i = 0; i < m.num_rows(); ++i) {
                data_[i + j * ldim_] = m(i, j);
            }
        }
    }

    /**
     * @brief Assign the data of another matrix to this matrix
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    matrix& operator=(const matrix& m)
    {
        assert(m.num_rows() == m_);
        assert(m.num_columns() == n_);
        for (int i = 0; i < m_; ++i) {
            for (int j = 0; j < n_; ++j) {
                data_[i + j * ldim_] = m(i, j);
            }
        }
        return *this;
    }

    /**
     * @brief Assign the data of another matrix to this matrix
     *        templated version
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    template <Matrix M>
    matrix& operator=(const M& m)
    {
        assert(m.num_rows() == m_);
        assert(m.num_columns() == n_);
        for (int i = 0; i < m_; ++i) {
            for (int j = 0; j < n_; ++j) {
                data_[i + j * ldim_] = m(i, j);
            }
        }
        return *this;
    }

    /**
     * @brief Assign the data of another matrix to this matrix
     *        Note, we do a deep copy and not a shallow copy
     *        even though the given matrix is temporary.
     *
     * @param m Matrix
     *          Matrix to copy
     * @return  Matrix&
     *          Reference to this matrix (used for chaining assignments)
     */
    matrix& operator=(matrix&& m)
    {
        assert(m.num_rows() == m_);
        assert(m.num_columns() == n_);
        for (int i = 0; i < m_; ++i) {
            for (int j = 0; j < n_; ++j) {
                data_[i + j * ldim_] = m(i, j);
            }
        }
        return *this;
    }

    // Returns the number of rows of the matrix
    inline idx_t num_rows() const { return m_; }

    // Returns the number of columns of the matrix
    inline idx_t num_columns() const { return n_; }

    // Returns the leading dimension of the matrix
    inline idx_t ldim() const { return ldim_; }

    // Returns the size of the matrix (i.e. the number of elements m * n)
    inline idx_t size() const { return n_ * m_; }

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
        assert(i < m_);
        assert(j < n_);
        return data_[i + j * ldim_];
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
        assert(i < m_);
        assert(j < n_);
        return data_[i + j * ldim_];
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
     * Matrix<float> A(5,5);
     * auto B = A.submatrix(1, 4, 1, 4); // B is a 3x3 submatrix of A
     *
     * Note: this function will also be called if the matrix is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline matrix submatrix(idx_t i1, idx_t i2, idx_t j1, idx_t j2) const
    {
        assert(i1 >= 0);
        assert(i2 <= m_);
        assert(j1 >= 0);
        assert(j2 <= n_);
        assert(i1 < i2);
        assert(j1 < j2);
        return matrix(i2 - i1, j2 - j1, &data_[i1 + j1 * ldim_], ldim_);
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
    inline vector<T> column(idx_t i) const
    {
        assert(i >= 0);
        assert(i < n_);
        return vector<T>(m_, &data_[i * ldim_], 1);
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
    inline vector<T> row(idx_t i) const
    {
        assert(i >= 0);
        assert(i < m_);
        return vector<T>(n_, &data_[i], ldim_);
    }

    /**
     * Return a pointer to the data of the matrix.
     */
    inline T* data() { return data_; }

    /**
     * Return a pointer to the data of the matrix.
     */
    inline const T* data() const { return data_; }

   private:
    // Number of rows of the matrix
    const idx_t m_;
    // Number of columns of the matrix
    const idx_t n_;
    // Leading dimension of the matrix
    const idx_t ldim_;
    // Whether the matrix owns the data or not
    const bool locally_allocated_;
    // Pointer to the data
    T* data_;
};

// Code for printing
template <typename T>
void print_matrix(const matrix<T>& m)
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
template <typename T>
void randomize(matrix<T>& m)
{
    for (int j = 0; j < m.num_columns(); ++j) {
        for (int i = 0; i < m.num_rows(); ++i) {
            m(i, j) = (T)rand() / RAND_MAX;
        }
    }
}

}  // namespace tws

#endif