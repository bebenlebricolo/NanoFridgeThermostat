cmake_minimum_required(VERSION 3.8)
project(NanoThermostat C CXX)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -Wall -Wextra")
    set(CMAKE_C_FLAGS_DEBUG     "-O0 -g3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")
    set(CMAKE_C_FLAGS_RELEASE   "-O3 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /Od /W4")
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /Od /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /W4 ")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /O2 /W4")
endif()

add_library(thermistor
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/interpolation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Thermistor/thermistor_ntc_100k_3950K.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Thermistor/thermistor.c

    ${CMAKE_CURRENT_SOURCE_DIR}/Math/interpolation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Thermistor/thermistor_ntc_100k_3950K.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Thermistor/thermistor.h
)

target_include_directories(thermistor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Thermistor
    ${CMAKE_CURRENT_SOURCE_DIR}/Math
)

# Add subdirectories (Tests)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Math/Tests
    ${CMAKE_CURRENT_BINARY_DIR}/Tests/Math
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thermistor/Tests
    ${CMAKE_CURRENT_BINARY_DIR}/Tests/Thermistor
)