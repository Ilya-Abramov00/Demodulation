cmake_minimum_required(VERSION 3.5)

project(DSP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC source/*.cpp include/${PROJECT_NAME}/*.h )

add_library(${PROJECT_NAME} SHARED ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC fftw3)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    include("unittest.cmake")
endif()
