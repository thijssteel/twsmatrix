#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "vector.hpp"

using namespace tws;

TEMPLATE_TEST_CASE("Vector class works", "[vector]", double, float, int)
{
    typedef TestType T;
    int n = 5;
    int stride = GENERATE(1,2);

    vector<T> v_large(n*stride);
    vector<T> v = v_large.subvector(0, n*stride, stride);
    for(int i = 0; i < v.size(); ++i) {
        v[i] = i;
    }

    REQUIRE(v.size() == n);

    SECTION("Test standard assignment"){
        for(int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == i);
        }
    }

    SECTION("Test copy constructor"){
        vector<T> v2(v);
        for(int i = 0; i < v.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
    }

    SECTION("Test move constructor"){
        vector<T> v2(std::move(v));
        for(int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
    }

    SECTION("Test copy assignment"){
        vector<T> v2(n);
        v2 = v;
        for(int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
    }

    SECTION("Test move assignment"){
        vector<T> v2(n);
        v2 = std::move(v);
        for(int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
    }

    SECTION("Test subvector"){
        vector<T> v2 = v.subvector(1, n-1);
        REQUIRE(v2.size() == n-2);
        for(int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i+1);
        }
    }

}