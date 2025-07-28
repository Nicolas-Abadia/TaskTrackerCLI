cmake_minimum_required(VERSION 3.10.0)
project(task-cli VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

# Main executable
add_executable(task-cli task-cli.cpp)

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)
include(GoogleTest)

# Create a library from task-cli.cpp (excluding main function)
# We need to separate the functions from main for testing
add_library(task_lib task-cli.cpp)
target_compile_definitions(task_lib PRIVATE TESTING_BUILD)

# Test executable - remove GTest::gtest_main since we have our own main
add_executable(test_task-cli test_task-cli.cpp)
target_link_libraries(test_task-cli 
    task_lib 
    GTest::gtest
)

# Discover tests
gtest_discover_tests(test_task-cli)

