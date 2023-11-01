cmake_minimum_required(VERSION 3.5)

project(Demodulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

if(NOT UNITTEST)
    option(UNITTEST ON)
endif()

if(UNITTEST)
    enable_testing()
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -g -O0")
    add_subdirectory(libs/googletest)
endif()

add_subdirectory(libs/Base)
add_subdirectory(libs/DSP)
add_subdirectory(libs/RTL-SDR-Receiver)

file(GLOB SRC include/${PROJECT_NAME}/*.h)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_executable(${PROJECT_NAME} ${SRC} main.cpp)
    target_link_libraries(${PROJECT_NAME} INTERFACE Base DSP)
    target_link_libraries(${PROJECT_NAME} PUBLIC receiver)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_library(${PROJECT_NAME} INTERFACE ${SRC})
    target_link_libraries(${PROJECT_NAME} INTERFACE Base receiver DSP)
    include("unittest.cmake")
endif()

target_include_directories(${PROJECT_NAME} INTERFACE include/)



