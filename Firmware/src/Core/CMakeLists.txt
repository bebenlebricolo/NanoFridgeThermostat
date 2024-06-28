cmake_minimum_required(VERSION 3.20)
project(NanoThermostat_CoreLib C CXX)

add_library(core STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/bridge.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/bridge.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/thermistor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/thermistor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/thermistor_ntc_100k_3950K.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/thermistor_ntc_100k_3950K.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/current.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/current.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Filters/sliding_average.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Filters/sliding_average.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/interpolation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Math/interpolation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/buttons.c
    ${CMAKE_CURRENT_SOURCE_DIR}/buttons.h
    ${CMAKE_CURRENT_SOURCE_DIR}/led.h
    ${CMAKE_CURRENT_SOURCE_DIR}/led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/spanner.c
    ${CMAKE_CURRENT_SOURCE_DIR}/spanner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mcu_time.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mcu_time.c
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Tests
    ${CMAKE_BINARY_DIR}/Tests
)

set_target_properties(core
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)