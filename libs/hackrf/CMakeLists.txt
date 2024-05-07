
project(hackrf LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC include/${PROJECT_NAME}/*.h source/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC hackrfAPI platform_SDR )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include("unittest.cmake")
