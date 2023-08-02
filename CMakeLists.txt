cmake_minimum_required(VERSION 3.5)

project(Demodulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(UNITTEST OFF)
enable_testing()


    
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

file(GLOB SRC include/${PROJECT_NAME}/*.h )
add_executable(${PROJECT_NAME} ${SRC} main.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE include/)

file(GLOB SOURCES "*.cpp") 

INCLUDE_DIRECTORIES(/usr/include)
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu)

add_subdirectory(libs/Base)
add_subdirectory(libs/googletest)
target_link_libraries(${PROJECT_NAME} INTERFACE fftw3 Base PUBLIC receiver)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/RTL-SDR-Receiver/src/cmake)
add_subdirectory(libs/RTL-SDR-Receiver/src)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

include("unittest.cmake")