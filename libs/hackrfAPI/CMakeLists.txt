
cmake_minimum_required(VERSION 3.21)

project(hackrfAPI)
file(GLOB_RECURSE  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.h )

add_library(${PROJECT_NAME} SHARED  ${SOURCE_FILES} )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)


find_package(PkgConfig)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBUSB libusb-1.0 IMPORTED_TARGET)
else()
    set(LIBUSB_LIBRARIES "" CACHE STRING "manual libusb path")
    set(LIBUSB_INCLUDE_DIRS "" CACHE STRING "manual libusb includepath")
endif()

if(PKG_CONFIG_FOUND AND NOT LIBUSB_FOUND)
    message(FATAL_ERROR "LibUSB 1.0 required to compile rtl-sdr")
endif()

# Dependencies
target_link_libraries(${PROJECT_NAME} ${LIBUSB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
   
