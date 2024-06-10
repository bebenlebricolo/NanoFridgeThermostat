cmake_minimum_required(VERSION 3.20)
project(NanoThermostat_UiLib C CXX)

add_library(ui STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/image_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/numbers_small.h
    ${CMAKE_CURRENT_SOURCE_DIR}/numbers_small.c
)

target_include_directories(ui PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(Tests ${CMAKE_CURRENT_BINARY_DIR}/Tests)
