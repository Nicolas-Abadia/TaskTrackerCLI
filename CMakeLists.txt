cmake_minimum_required(VERSION 3.10.0)
project(task-cli VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

# Main executable
add_executable(task-cli 
    src/main.cpp
    src/cli.cpp
    src/json.cpp
    src/utils.cpp
)

