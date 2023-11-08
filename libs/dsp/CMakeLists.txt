cmake_minimum_required(VERSION 3.5)

project(dsp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC source/*.cpp include/${PROJECT_NAME}/*.h )

add_library(${PROJECT_NAME} SHARED ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC fftw3 base)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include("unittest.cmake")

