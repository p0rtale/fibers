cmake_minimum_required(VERSION 3.14)

project(fibers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror -Wpedantic)

add_subdirectory(fibers)
add_subdirectory(examples)
