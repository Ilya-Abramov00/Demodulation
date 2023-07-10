cmake_minimum_required(VERSION 3.5)

project(Demodulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(UNITTEST ON)
enable_testing()

file(GLOB SRC include/${PROJECT_NAME}/*.h )
add_library(${PROJECT_NAME} INTERFACE ${SRC})

target_include_directories(${PROJECT_NAME} INTERFACE include/)

add_subdirectory(libs/Base)
add_subdirectory(libs/googletests)
target_link_libraries(${PROJECT_NAME} INTERFACE Base)

include("unittest.cmake")
