add_library(thermistor STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor_ntc_100k_3950K.c
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor_ntc_100k_3950K.h
)

target_link_libraries(thermistor mathlib)

target_include_directories(thermistor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests
    ${CMAKE_CURRENT_BINARY_DIR}/Tests
)

