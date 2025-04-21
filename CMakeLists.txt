cmake_minimum_required(VERSION 3.30)
project(quotes_moubarac_snani_E C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Main_projet src/main.c src/bmp8.c)
add_executable(test tests/test_1.c src/bmp8.c)
