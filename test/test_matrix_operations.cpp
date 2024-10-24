#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "twsmatrix.hpp"

using namespace tws;

TEMPLATE_TEST_CASE("Matrix operations work", "[matrix]", double, float, int)
{
    typedef TestType T;
    int m = GENERATE(1,2,4,10);
    int n = GENERATE(1,2,4,10);
    int k = GENERATE(1,2,4,10);

    matrix<T> A(m, k);
    matrix<T> B(k, n);
    vector<T> v(k);

    for(int j = 0; j < A.num_columns(); ++j) {
        for(int i = 0; i < A.num_rows(); ++i) {
            A(i,j) = i + j;
        }
    }

    for(int j = 0; j < B.num_columns(); ++j) {
        for(int i = 0; i < B.num_rows(); ++i) {
            B(i,j) = i - j;
        }
    }

    for(int i = 0; i < v.size(); ++i) {
        v[i] = i;
    }

    SECTION("Test matrix-vector multiplication"){
        auto b = A * v;

        for(int i = 0; i < A.num_rows(); ++i) {
            T sum = 0;
            for(int j = 0; j < A.num_columns(); ++j) {
                sum += A(i,j) * v[j];
            }
            REQUIRE(b[i] == sum);
        }

        vector<T> c(A.num_rows());
        multiply(A, v, c);

        for(int i = 0; i < A.num_rows(); ++i) {
            T sum = 0;
            for(int j = 0; j < A.num_columns(); ++j) {
                sum += A(i,j) * v[j];
            }
            REQUIRE(c[i] == sum);
        }
    }

    SECTION("Test matrix-matrix multiplication"){
        auto C = A * B;

        for(int i = 0; i < A.num_rows(); ++i) {
            for(int j = 0; j < B.num_columns(); ++j) {
                T sum = 0;
                for(int k = 0; k < A.num_columns(); ++k) {
                    sum += A(i,k) * B(k,j);
                }
                REQUIRE(C(i,j) == sum);
            }
        }

        matrix<T> D(A.num_rows(), B.num_columns());
        multiply(A, B, D);

        for(int i = 0; i < A.num_rows(); ++i) {
            for(int j = 0; j < B.num_columns(); ++j) {
                T sum = 0;
                for(int k = 0; k < A.num_columns(); ++k) {
                    sum += A(i,k) * B(k,j);
                }
                REQUIRE(D(i,j) == sum);
            }
        }
    }

    SECTION("Test transpose"){
        auto At = transpose(A);

        for(int i = 0; i < A.num_rows(); ++i) {
            for(int j = 0; j < A.num_columns(); ++j) {
                REQUIRE(At(j,i) == A(i,j));
            }
        }

        matrix<T> Bt(A.num_columns(), A.num_rows());
        transpose(A, Bt);

        for(int i = 0; i < A.num_rows(); ++i) {
            for(int j = 0; j < A.num_columns(); ++j) {
                REQUIRE(Bt(j,i) == A(i,j));
            }
        }
    }
}