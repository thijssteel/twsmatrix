cmake_minimum_required(VERSION 3.29)
project(1dplasmasolver)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)
add_executable(tests test/test_vector.cpp test/test_matrix.cpp test/test_vector_operations.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(tests PRIVATE include)