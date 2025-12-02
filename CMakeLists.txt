cmake_minimum_required(VERSION 3.14)

project(ThreadPool_Cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(ThreadPoolApp src/main.cpp src/ThreadPool.cpp)

enable_testing()

add_executable(ThreadPoolTests 
    tests/ThreadPoolTests.cpp 
    src/ThreadPool.cpp 
)

target_link_libraries(ThreadPoolTests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(ThreadPoolTests)