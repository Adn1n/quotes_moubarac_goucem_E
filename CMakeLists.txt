cmake_minimum_required(VERSION 3.30)
project(quotes_moubarac_snani_E C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

add_executable(main main.c src/bmp8.c src/bmp24.c
        utils.c
        src/histogramme.c

)
add_executable(test_8bits tests/test_8bit.c src/bmp8.c
        utils.c src/histogramme.c
)
add_executable(test_24bits tests/test_24bit.c src/bmp24.c
        utils.c src/histogramme.c
)
