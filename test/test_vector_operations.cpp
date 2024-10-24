#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "vector.hpp"
#include "vector_operations.hpp"

using namespace tws;

TEMPLATE_TEST_CASE("Vector operations work", "[vector]", double, float, int)
{
    typedef TestType T;
    int n = 5;

    vector<T> a(n);
    vector<T> b(n);

    for(int i = 0; i < n; ++i) {
        a[i] = i;
        b[i] = n-i;
    }

    SECTION("Test vector addition"){
        auto c = a + b;
        for(int i = 0; i < n; ++i) {
            REQUIRE(c[i] == n);
        }

        vector<T> d(n);
        add(a, b, d);
        for(int i = 0; i < n; ++i) {
            REQUIRE(d[i] == n);
        }

        a += b;
        for(int i = 0; i < n; ++i) {
            REQUIRE(a[i] == n);
        }
    }

    SECTION("Test vector subtraction"){
        auto c = a - b;
        for(int i = 0; i < n; ++i) {
            REQUIRE(c[i] == 2*i - n);
        }

        vector<T> d(n);
        sub(a, b, d);
        for(int i = 0; i < n; ++i) {
            REQUIRE(d[i] == 2*i - n);
        }

        a -= b;
        for(int i = 0; i < n; ++i) {
            REQUIRE(a[i] == 2*i - n);
        }
    }

    SECTION("Test vector multiplication"){
        T scalar = 2;
        auto c = a * scalar;
        for(int i = 0; i < n; ++i) {
            REQUIRE(c[i] == 2*i);
        }

        vector<T> d(n);
        multiply(a, scalar, d);
        for(int i = 0; i < n; ++i) {
            REQUIRE(d[i] == 2*i);
        }

        a *= scalar;
        for(int i = 0; i < n; ++i) {
            REQUIRE(a[i] == 2*i);
        }
    }

    SECTION("Test dot"){
        T c = dot(a, b);
        T c_ref = 0;
        for(int i = 0; i < n; ++i) {
            c_ref += a[i] * b[i];
        }

        REQUIRE(c == c_ref);
    }

    SECTION("Test norm"){
        T c = norm(a);
        T c_ref = 0;
        for(int i = 0; i < n; ++i) {
            c_ref += a[i] * a[i];
        }
        c_ref = sqrt(c_ref);

        REQUIRE(c == c_ref);
    }

}