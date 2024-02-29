cmake_minimum_required(VERSION 3.20)
project(NanoThermostat_HalLib C CXX)

add_library(hal STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/persistent_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/persistent_memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timebase.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timebase.h
)
