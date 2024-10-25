#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "twsmatrix.hpp"

using namespace tws;

template <Matrix M>
void add_one_ref(M& m)
{
    for (int j = 0; j < m.num_columns(); ++j) {
        for (int i = 0; i < m.num_rows(); ++i) {
            m(i, j) += 1;
        }
    }
}

template <Matrix M>
void add_one_value(M m)
{
    for (int j = 0; j < m.num_columns(); ++j) {
        for (int i = 0; i < m.num_rows(); ++i) {
            m(i, j) += 1;
        }
    }
}

TEMPLATE_TEST_CASE("Matrix class works", "[matrix]", double, float, int)
{
    typedef TestType T;
    int m = GENERATE(1, 2, 4, 10);
    int n = GENERATE(1, 2, 4, 10);

    matrix<T> A(m, n);

    REQUIRE(A.num_rows() == m);
    REQUIRE(A.num_columns() == n);

    for (int j = 0; j < A.num_columns(); ++j) {
        for (int i = 0; i < A.num_rows(); ++i) {
            A(i, j) = i + j;
        }
    }

    SECTION("Test standard assignment")
    {
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == i + j);
            }
        }
    }

    SECTION("Test copy constructor")
    {
        matrix<T> B(A);
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
        // Make sure the data is not shared
        REQUIRE(B.data() != A.data());
    }

    SECTION("Test move constructor")
    {
        T* data = A.data();
        matrix<T> B(std::move(A));
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
        // Make sure the data is shared
        REQUIRE(B.data() == data);
    }

    SECTION("Test copy assignment")
    {
        matrix<T> B(m, n);
        B = A;
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
        // Make sure the data is not shared
        REQUIRE(B.data() != A.data());
    }

    SECTION("Test move assignment")
    {
        matrix<T> B(m, n);
        T* data = A.data();
        B = std::move(A);
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
        // Make sure the data is shared
        REQUIRE(B.data() == data);
    }

    SECTION("Test row")
    {
        for (int i = 0; i < A.num_rows(); ++i) {
            vectorview<T> row = A.row(i);
            for (int j = 0; j < A.num_columns(); ++j) {
                REQUIRE(row[j] == i + j);
            }
            // Modify row and make sure A is modified as well
            for (int j = 0; j < A.num_columns(); ++j) {
                row[j] = 10 * j;
            }
            for (int j = 0; j < A.num_columns(); ++j) {
                REQUIRE(A(i, j) == 10 * j);
            }
        }
    }

    SECTION("Test column")
    {
        for (int j = 0; j < A.num_columns(); ++j) {
            vectorview<T> column = A.column(j);
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(column[i] == i + j);
            }
            // Modify column and make sure A is modified as well
            for (int i = 0; i < A.num_rows(); ++i) {
                column[i] = 10 * i;
            }
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == 10 * i);
            }
        }
    }

    SECTION("Test submatrix")
    {
        if (m > 2 and n > 2) {
            matrixview<T> B = A.submatrix(1, m - 1, 1, n - 1);
            for (int j = 0; j < B.num_columns(); ++j) {
                for (int i = 0; i < B.num_rows(); ++i) {
                    REQUIRE(B(i, j) == i + j + 2);
                }
            }
            // Modify B and make sure A is modified as well
            for (int j = 0; j < B.num_columns(); ++j) {
                for (int i = 0; i < B.num_rows(); ++i) {
                    B(i, j) = 2 * i + j * 3;
                }
            }
            for (int j = 0; j < B.num_columns(); ++j) {
                for (int i = 0; i < B.num_rows(); ++i) {
                    REQUIRE(A(i + 1, j + 1) == 2 * i + j * 3);
                }
            }
        }
    }

    SECTION("Test add_one_ref")
    {
        add_one_ref(A);
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == i + j + 1);
            }
        }
    }

    SECTION("Test add_one_value")
    {
        add_one_value(A);
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == i + j);
            }
        }
    }
}

TEMPLATE_TEST_CASE("Matrixview class works", "[matrix]", double, float, int)
{
    typedef TestType T;
    int m = GENERATE(1, 2, 4, 10);
    int n = GENERATE(1, 2, 4, 10);
    int ldim = m + GENERATE(0, 1);
    int offset = GENERATE(0, 3);

    std::shared_ptr<T[]> data(new T[offset + ldim * n]);

    matrixview<T> A(m, n, data, ldim, offset);

    REQUIRE(A.num_rows() == m);
    REQUIRE(A.num_columns() == n);
    REQUIRE(A.ldim() == ldim);
    REQUIRE(A.offset() == offset);

    for (int j = 0; j < A.num_columns(); ++j) {
        for (int i = 0; i < A.num_rows(); ++i) {
            A(i, j) = i + j;
        }
    }

    SECTION("Test standard assignment")
    {
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == i + j);
            }
        }
    }

    SECTION("Test copy constructor")
    {
        matrixview<T> B(A);
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
        // Make sure the data is shared
        REQUIRE(B.data() == A.data());
    }

    SECTION("Test move constructor")
    {
        matrixview<T> B(std::move(A));
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
    }

    SECTION("Test copy assignment")
    {
        matrixview<T> B(m, n, data, ldim, offset);
        B = A;
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
        // Make sure the data is shared
        REQUIRE(B.data() == A.data());
    }

    SECTION("Test move assignment")
    {
        matrixview<T> B(m, n, data, ldim, offset);
        B = std::move(A);
        for (int j = 0; j < B.num_columns(); ++j) {
            for (int i = 0; i < B.num_rows(); ++i) {
                REQUIRE(B(i, j) == i + j);
            }
        }
    }

    SECTION("Test row")
    {
        for (int i = 0; i < A.num_rows(); ++i) {
            vectorview<T> row = A.row(i);
            for (int j = 0; j < A.num_columns(); ++j) {
                REQUIRE(row[j] == i + j);
            }
            // Modify row and make sure A is modified as well
            for (int j = 0; j < A.num_columns(); ++j) {
                row[j] = 10 * j;
            }
            for (int j = 0; j < A.num_columns(); ++j) {
                REQUIRE(A(i, j) == 10 * j);
            }
        }
    }

    SECTION("Test column")
    {
        for (int j = 0; j < A.num_columns(); ++j) {
            vectorview<T> column = A.column(j);
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(column[i] == i + j);
            }
            // Modify column and make sure A is modified as well
            for (int i = 0; i < A.num_rows(); ++i) {
                column[i] = 10 * i;
            }
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == 10 * i);
            }
        }
    }

    SECTION("Test submatrix")
    {
        if (m > 2 and n > 2) {
            matrixview<T> B = A.submatrix(1, m - 1, 1, n - 1);
            for (int j = 0; j < B.num_columns(); ++j) {
                for (int i = 0; i < B.num_rows(); ++i) {
                    REQUIRE(B(i, j) == i + j + 2);
                }
            }
            // Modify B and make sure A is modified as well
            for (int j = 0; j < B.num_columns(); ++j) {
                for (int i = 0; i < B.num_rows(); ++i) {
                    B(i, j) = 2 * i + j * 3;
                }
            }
            for (int j = 0; j < B.num_columns(); ++j) {
                for (int i = 0; i < B.num_rows(); ++i) {
                    REQUIRE(A(i + 1, j + 1) == 2 * i + j * 3);
                }
            }
        }
    }

    SECTION("Test add_one_ref")
    {
        add_one_ref(A);
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == i + j + 1);
            }
        }
    }

    SECTION("Test add_one_value")
    {
        add_one_value(A);
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                // This may seem counterintuitive, but if the matrixview is
                // passed by value, we expect the original data to be modified.
                REQUIRE(A(i, j) == i + j + 1);
            }
        }
    }
}

TEMPLATE_TEST_CASE(
    "Matrixview - matrix interaction works", "[matrix]", double, float, int)
{
    typedef TestType T;
    int m = GENERATE(1, 2, 4, 10);
    int n = GENERATE(1, 2, 4, 10);
    int ldim = m + GENERATE(0, 1);
    int offset = GENERATE(0, 3);

    matrix<T> A(m, n);

    for (int j = 0; j < A.num_columns(); ++j) {
        for (int i = 0; i < A.num_rows(); ++i) {
            A(i, j) = i + j;
        }
    }

    std::shared_ptr<T[]> data(new T[offset + ldim * n]);
    matrixview<T> B(m, n, data, ldim, offset);

    for (int j = 0; j < B.num_columns(); ++j) {
        for (int i = 0; i < B.num_rows(); ++i) {
            B(i, j) = 10 * i + 3 * j;
        }
    }

    SECTION("Test matrixview -> matrix")
    {
        A = B;
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == 10 * i + 3 * j);
            }
        }
        matrix<T> C(B);
        for (int j = 0; j < C.num_columns(); ++j) {
            for (int i = 0; i < C.num_rows(); ++i) {
                REQUIRE(C(i, j) == 10 * i + 3 * j);
            }
        }
    }
}

TEMPLATE_TEST_CASE("Matrix utils work", "[matrix]", double, float, int)
{
    typedef TestType T;
    int m = 4;
    int n = 3;

    matrix<T> A(m, n);
    randomize(A);
    print_matrix(A);

    std::shared_ptr<T[]> data(new T[m * n]);
    matrixview<T> B(m, n, data, m, 0);

    randomize(B);
    print_matrix(B);
}

TEMPLATE_TEST_CASE("Test some weird behavior with matrix r-values",
                   "[matrix]",
                   double,
                   float,
                   int)
{
    typedef TestType T;
    int m = 2;
    int n = 3;

    matrix<T> A(m, n);

    for (int j = 0; j < A.num_columns(); ++j) {
        for (int i = 0; i < A.num_rows(); ++i) {
            A(i, j) = 0;
        }
    }

    SECTION("Check if view is modified if passed by value")
    {
        // This is expected behavior, we pass a view which should always be a
        // reference even if passed by value.
        add_one_value(A.submatrix(0, m, 0, n));
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == 1);
            }
        }
    }

    SECTION("Check that matrix is never modified, even for r-value views")
    {
        // If we explicitly ask for a vector to be passed by value
        // it should be copied and not modified.
        add_one_value<matrix<T>>(A.submatrix(0, m, 0, n));
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == 0);
            }
        }
    }

    SECTION("Check that matrix can be passed to view")
    {
        add_one_value<matrixview<T>>(A);
        for (int j = 0; j < A.num_columns(); ++j) {
            for (int i = 0; i < A.num_rows(); ++i) {
                REQUIRE(A(i, j) == 1);
            }
        }
    }
}