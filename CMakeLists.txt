cmake_minimum_required(VERSION 3.2)
project(constexpr_murmur3_hash)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCE_FILES test.cc)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} constexpr_murmur3.h)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
