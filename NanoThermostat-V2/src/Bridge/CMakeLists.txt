add_library(bridge STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge.c
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge.h
)

target_include_directories(bridge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Tests
    ${CMAKE_CURRENT_BINARY_DIR}/Tests
)
