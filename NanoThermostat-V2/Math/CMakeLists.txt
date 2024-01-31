add_library(mathlib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolation.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolation.h
)

target_include_directories(mathlib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests
    ${CMAKE_CURRENT_BINARY_DIR}/Tests
)
