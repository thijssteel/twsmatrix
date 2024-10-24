#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "twsmatrix.hpp"

using namespace tws;

template <Vector V>
void add_one_ref(V& v)
{
    for (int i = 0; i < v.size(); ++i) {
        v[i] += 1;
    }
}

template <Vector V>
void add_one_value(V v)
{
    for (int i = 0; i < v.size(); ++i) {
        v[i] += 1;
    }
}

TEMPLATE_TEST_CASE("Vector class works", "[vector]", double, float, int)
{
    typedef TestType T;
    int n = 5;

    vector<T> v(n);
    for (int i = 0; i < v.size(); ++i) {
        v[i] = i;
    }

    REQUIRE(v.size() == n);

    SECTION("Test standard assignment")
    {
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == i);
        }
    }

    SECTION("Test copy constructor")
    {
        vector<T> v2(v);
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the data is not shared
        REQUIRE(v2.data() != v.data());
    }

    SECTION("Test move constructor")
    {
        T* data = v.data();
        vector<T> v2(std::move(v));
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the data is shared
        REQUIRE(v2.data() == data);
    }

    SECTION("Test copy assignment")
    {
        vector<T> v2(n);
        T* data1 = v.data();
        T* data2 = v2.data();
        v2 = v;
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the pointers themselves are not modified
        REQUIRE(v2.data() == data2);
        REQUIRE(v.data() == data1);
    }

    SECTION("Test move assignment")
    {
        vector<T> v2(n);
        T* data = v.data();
        v2 = std::move(v);
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the pointer has been moved
        REQUIRE(v2.data() == data);
    }

    SECTION("Test subvector")
    {
        vectorview<T> v2 = v.subvector(1, n-1);
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i + 1);
        }
        // Modify v2 and make sure v is modified as well
        for( int i = 0; i < v2.size(); ++i){
            v2[i] += 1;
        }
        for (int i = 1; i < v.size() - 1; ++i) {
            REQUIRE(v[i] == i + 1);
        }
    }

    SECTION("Test add_one_ref")
    {
        add_one_ref(v);
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == i + 1);
        }
    }

    SECTION("Test add_one_value")
    {
        add_one_value(v);
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == i);
        }
    }
}

TEMPLATE_TEST_CASE("Vectorview class works", "[vector]", double, float, int)
{
    typedef TestType T;
    int n = 5;
    int stride = GENERATE(1, 2);

    std::shared_ptr<T[]> data(new T[n * stride]);

    vectorview<T> v(n, data, stride);

    REQUIRE(v.size() == n);

    for (int i = 0; i < v.size(); ++i) {
        v[i] = i;
    }

    SECTION("Test standard assignment")
    {
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == i);
        }
    }

    SECTION("Test copy constructor")
    {
        vectorview<T> v2(v);
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the data is shared
        REQUIRE(v2.data() == v.data());
    }

    SECTION("Test move constructor")
    {
        T* data = v.data();
        vectorview<T> v2(std::move(v));
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the data is shared
        REQUIRE(v2.data() == data);
    }

    SECTION("Test copy assignment")
    {
        std::shared_ptr<T[]> data_new(new T[n * stride]);
        vectorview<T> v2(n, data_new, stride);
        T* data1 = v.data();
        v2 = v;
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the pointers has been copied
        REQUIRE(data_new.use_count() == 1);
        REQUIRE(v2.data() == data1);
        REQUIRE(v.data() == data1);
    }

    SECTION("Test move assignment")
    {
        std::shared_ptr<T[]> data_new(new T[n * stride]);
        vectorview<T> v2(n, data_new, stride);
        T* data = v.data();
        v2 = std::move(v);
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i);
        }
        // Make sure the pointer has been moved
        REQUIRE(data_new.use_count() == 1);
        REQUIRE(v2.data() == data);
    }

    SECTION("Test subvector")
    {
        vectorview<T> v2 = v.subvector(1, n-1);
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == i + 1);
        }
        // Modify v2 and make sure v is modified as well
        for( int i = 0; i < v2.size(); ++i){
            v2[i] += 1;
        }
        for (int i = 1; i < v.size() - 1; ++i) {
            REQUIRE(v[i] == i + 1);
        }
    }

    SECTION("Test add_one_ref")
    {
        add_one_ref(v);
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == i + 1);
        }
    }

    SECTION("Test add_one_value")
    {
        add_one_value(v);
        for (int i = 0; i < v.size(); ++i) {
            // This may seem counterintuitive, but if the vectorview is passed
            // by value, we expect the original data to be modified.
            REQUIRE(v[i] == i + 1);
        }
    }
}

TEMPLATE_TEST_CASE("Vectorview - vector interaction works", "[vector]", double, float, int)
{
    typedef TestType T;
    int n = 5;
    int stride = GENERATE(1, 2);

    vector<T> v(n);
    std::shared_ptr<T[]> data(new T[n * stride]);
    vectorview<T> vv(n, data, stride);

    for (int i = 0; i < v.size(); ++i) {
        v[i] = i;
    }

    for (int i = 0; i < vv.size(); ++i) {
        vv[i] = 10 * i;
    }

    SECTION("Test vectorview -> vector")
    {
        v = vv;
        for (int i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == 10 * i);
        }
        vector<T> v2(vv);
        REQUIRE(v2.size() == vv.size());
        for (int i = 0; i < v2.size(); ++i) {
            REQUIRE(v2[i] == 10 * i);
        }

    }
}