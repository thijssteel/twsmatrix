
#ifndef vector_hpp
#define vector_hpp

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>

namespace tws {

// This defines a vector as an abstract concept.
// You can safely ignore this untill you have learned about concepts.
#if(__cplusplus >= 202002L)

template <typename V>
concept Vector = requires(V v)
{
    {typename V::val_t()};
    {v.size()};
    {v[0]};
};

#else

#define Vector typename

#endif

/**
 * A vector class that can be used to store vectors of arbitrary size.
 *
 * @tparam T this is a template parameter that specifies the type of the
 *           elements of the vector.
 */
template <typename T = double>
class vector {
   public:
    typedef T val_t;
    typedef int idx_t;

   public:
    /**
     * @brief Construct a new vector
     *
     * @param n  integer
     *           Number of elements of the vector
     */
    vector(idx_t n)
        : n_(n), stride_(1), locally_allocated_(true), data_(new T[n_])
    {
#ifndef NDEBUG
        std::fill(data_, data_ + n_, NAN);
#endif
    }

    /**
     * @brief Construct a new vector without allocating memory
     *
     * @param n      integer
     *               Number of elements of the vector
     *
     * @param data   pointer to the data
     *               Pointer to the data of the vector.
     *               Note: this constructor does not check whether the data
     *               pointed to by ptr is large enough to store the vector.
     *               Use this constructor with caution.
     *
     * @param stride integer
     *               Stride of the vector (number of position between two
     * elements)
     */
    vector(idx_t n, T* data, idx_t stride)
        : n_(n), stride_(stride), locally_allocated_(false), data_(data)
    {}

    // Destructor
    ~vector()
    {
        if (locally_allocated_) {
            delete[] data_;
        }
    }

    /**
     * @brief Construct a new vector as a copy of another vector
     *
     * Note: this contructor makes a deep copy of the vector and allocates
     * new memory to facilitate this.
     *
     * @param v vector
     *          vector to copy
     */
    vector(const vector& v)
        : n_(v.size()), stride_(1), locally_allocated_(true), data_(new T[n_])
    {
        for (int i = 0; i < n_; ++i) {
            data_[i] = v[i];
        }
    }

    /**
     * @brief Construct a new vector as a copy of another vector
     *
     * Note: this contructor makes a deep copy of the vector and allocates
     * new memory to facilitate this.
     *
     * @param v vector
     *          vector to copy
     */
    template <Vector V>
    vector(const V& v)
        : n_(v.size()), stride_(1), locally_allocated_(true), data_(new T[n_])
    {
        for (int i = 0; i < n_; ++i) {
            data_[i] = v[i];
        }
    }

    /**
     * @brief Construct a new vector object by moving the data of another vector
     *        Note, we do a deep copy and not a shallow copy
     *        even though the given vector is temporary.
     *
     * @param v vector
     *          vector to move
     */
    vector(vector&& v)
        : n_(v.size()),
          stride_(1),
          locally_allocated_(true),
          data_(new T[n_])
    {
        for (int i = 0; i < n_; ++i) {
            data_[i] = v[i];
        }
    }

    /**
     * @brief Assign the data of another vector to this vector
     *
     * @param v vector
     *          vector to copy
     * @return  vector&
     *          Reference to this vector (used for chaining assignments)
     */
    vector& operator=(const vector<T>& v)
    {
        assert(v.size() == n_);
        for (int i = 0; i < n_; ++i) {
            data_[i * stride_] = v[i];
        }
        return *this;
    }

    /**
     * @brief Assign the data of another vector to this vector
     *        templated version
     *
     * @param v vector
     *          vector to copy
     * @return  vector&
     *          Reference to this vector (used for chaining assignments)
     */
    template <Vector V>
    vector& operator=(const V& v)
    {
        assert(v.size() == n_);
        for (int i = 0; i < n_; ++i) {
            data_[i * stride_] = v[i];
        }
        return *this;
    }

    /**
     * @brief Assign the data of another vector to this vector
     *        Note, we do a deep copy and not a shallow copy
     *        even though the given vector is temporary.
     *
     * @param v vector
     *          vector to move
     * @return  vector&
     *          Reference to this vector (used for chaining assignments)
     */
    vector& operator=(vector&& v)
    {
        assert(v.size() == n_);
        for (int i = 0; i < n_; ++i) {
            data_[i * stride_] = v[i];
        }
        return *this;
    }

    // Returns the size of the vector
    inline idx_t size() const { return n_; }

    // Returns the stride of the vector
    inline idx_t stride() const { return stride_; }

    /**
     * @brief Access the i-th element of the vector
     *
     * @param i integer
     *          index
     * @return  const T&
     *          Because this vector is const, we return a const reference.
     */
    inline const T& operator[](const idx_t i) const
    {
        assert(i < n_);
        return data_[i * stride_];
    }

    /**
     * @brief Access the i-th element of the vector
     *
     * @param i integer
     *          index
     * @return  T&
     */
    inline T& operator[](const idx_t i)
    {
        assert(i < n_);
        return data_[i * stride_];
    }

    /**
     * Return a subvector of the vector.
     * The subvector starts at index start and ends at index end - 1.
     * The stride is the step size between two elements of the subvector.
     *
     * @param start  first index of the subvector
     * @param end    last index of the subvector (not inclusive)
     * @param stride step size between two elements of the subvector
     *
     * @example
     * vector<float> v(10);
     * auto v2 = v.subvector(1,9,2); // v2 is a length 4 vector containing the
     * elements v[1], v[3], v[5], v[7]
     *
     * Note: this function will also be called if the vector is const, even
     * though the returned object is not const. Solving this problem would
     * require either an inefficient or a more complex design.
     */
    inline vector subvector(idx_t start, idx_t end, idx_t stride = 1) const
    {
        assert(start >= 0);
        assert(end <= n_);
        assert(start < end);
        assert(stride > 0);
        return vector((end - start) / stride, &data_[start * stride_],
                      stride * stride_);
    }

    /**
     * Return a pointer to the data of the vector.
     */
    inline const T* data() const { return data_; }

    /**
     * Return a pointer to the data of the vector.
     */
    inline T* data() { return data_; }

    /**
     * Return a pointer to the beginning of the vector.
     */
    inline T* begin(){
        return data_;
    }

    /**
     * Return a pointer to the end of the vector.
     */
    inline T* end(){
        return data_+n_;
    }

    /**
     * Return a const pointer to the beginning of the vector.
     */
    inline const T* cbegin() const{
        return data_;
    }

    /**
     * Return a const pointer to the end of the vector.
     */
    inline const T* cend() const{
        return data_+n_;
    }

   private:
    const idx_t n_;
    const idx_t stride_;
    const bool locally_allocated_;
    T* data_;
};

// Initialize a vector with random values
template <typename T>
void randomize(vector<T>& v)
{
    for (int i = 0; i < v.size(); ++i) {
        v[i] = (T)rand() / RAND_MAX;
    }
}

// Code for printing
template <typename T>
void print_vector(const vector<T>& v)
{
    std::cout << "(" << v.size() << ")[" << std::endl;
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << std::endl;
    }
    std::cout << "]" << std::endl;
}

}  // namespace tws

#endif