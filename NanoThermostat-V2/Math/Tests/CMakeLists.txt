add_executable(interpolation_test
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolation_test.cpp
)

gtest_discover_tests(interpolation_test)

target_include_directories(interpolation_test
    SYSTEM PUBLIC
        ${GTEST_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(interpolation_test
    mathlib
    ${GTEST_LIBRARIES}
)
