cmake_minimum_required(VERSION 3.5)

project(demodulation)

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
add_subdirectory(libs/dsp)
add_subdirectory(libs/RTL-SDR-Receiver)

file(GLOB SRC include/${PROJECT_NAME}/*.h)

add_library(${PROJECT_NAME} INTERFACE ${SRC})
target_link_libraries(${PROJECT_NAME} INTERFACE base receiver dsp)

target_include_directories(${PROJECT_NAME} INTERFACE libs/RTL-SDR-Receiver/include)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include("unittest.cmake")

add_executable(Demodulator main.cpp)
target_link_libraries(Demodulator PUBLIC receiver dsp base demodulation)

