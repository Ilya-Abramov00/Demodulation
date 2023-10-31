cmake_minimum_required(VERSION 3.5)

project(DSP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC include/${PROJECT_NAME}/*.h )
add_library(${PROJECT_NAME} INTERFACE ${SRC})

target_link_libraries(${PROJECT_NAME} INTERFACE fftw3)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    include("unittest.cmake")
endif()
