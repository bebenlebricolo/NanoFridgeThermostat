
######################################################################
########################### Numbers tests ############################
######################################################################
add_compile_definitions(UNIT_TESTING)

add_executable(numbers_small_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/numbers_small_tests.cpp
)

gtest_discover_tests(numbers_small_tests )

target_include_directories(numbers_small_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(numbers_small_tests
    ui
    GTest::gtest
)

set_target_properties(numbers_small_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)
