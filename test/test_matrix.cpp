#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "matrix.hpp"

using namespace tws;

TEMPLATE_TEST_CASE("Matrix class works", "[matrix]", double, float, int)
{
    typedef TestType T;
    int m = GENERATE(1,2,4,10);
    int n = GENERATE(1,2,4,10);
    int ldim = m + GENERATE(0,1);

    matrix<T> A_large(ldim, n);
    matrix<T> A = A_large.submatrix(0, m, 0, n);

    REQUIRE(A.num_rows() == m);
    REQUIRE(A.num_columns() == n);

    // Initialize the matrix
    for(int j = 0; j < A.num_columns(); ++j) {
        for(int i = 0; i < A.num_rows(); ++i) {
            A(i,j) = i + j;
        }
    }

    SECTION("Test standard assignment"){
        for(int j = 0; j < A.num_columns(); ++j) {
            for(int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i,j) == i + j);
            }
        }
    }

    SECTION("Test copy constructor"){
        matrix<T> A2(A);
        for(int j = 0; j < A.num_columns(); ++j) {
            for(int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A2(i,j) == i + j);
            }
        }
    }

    SECTION("Test move constructor"){
        matrix<T> A2(std::move(A));
        for(int j = 0; j < A2.num_columns(); ++j) {
            for(int i = 0; i < A2.num_rows(); ++i) {
                REQUIRE(A2(i,j) == i + j);
            }
        }
    }

    SECTION("Test copy assignment"){
        matrix<T> A2(m, n);
        A2 = A;
        for(int j = 0; j < A2.num_columns(); ++j) {
            for(int i = 0; i < A2.num_rows(); ++i) {
                REQUIRE(A2(i,j) == i + j);
            }
        }
    }

    SECTION("Test move assignment"){
        matrix<T> A2(m, n);
        A2 = std::move(A);
        for(int j = 0; j < A2.num_columns(); ++j) {
            for(int i = 0; i < A2.num_rows(); ++i) {
                REQUIRE(A2(i,j) == i + j);
            }
        }
    }

    SECTION("Test submatrix"){
        if(m > 2 and n > 2){
            matrix<T> A2 = A.submatrix(1, m-1, 1, n-1);
            REQUIRE(A2.num_rows() == m-2);
            REQUIRE(A2.num_columns() == n-2);
            for(int j = 0; j < A2.num_columns(); ++j) {
                for(int i = 0; i < A2.num_rows(); ++i) {
                    REQUIRE(A2(i,j) == i + j + 2);
                }
            }
        }
    }



}