######################################################################
############################ Bridge tests ############################
######################################################################

add_executable(bridge_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge_tests.cpp
)

gtest_discover_tests(bridge_tests )

target_include_directories(bridge_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(bridge_tests
    core
    GTest::gtest
)

set_target_properties(bridge_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)


######################################################################
############################ Bridge tests ############################
######################################################################

add_executable(interpolation_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolation_tests.cpp
)

gtest_discover_tests(interpolation_tests)

target_include_directories(interpolation_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(interpolation_tests
    core
    GTest::gtest
)

set_target_properties(interpolation_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

######################################################################
######################### Thermistor tests ###########################
######################################################################

add_executable(thermistor_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor_tests.cpp
)

gtest_discover_tests(thermistor_tests)

target_include_directories(thermistor_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(thermistor_tests
    core
    GTest::gtest
)

set_target_properties(thermistor_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

######################################################################
######################### Current tests ##############################
######################################################################

add_executable(current_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/current_tests.cpp
)

gtest_discover_tests(current_tests)

target_include_directories(current_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(current_tests
    core
    GTest::gtest
)

set_target_properties(current_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

######################################################################
############################# Led tests ##############################
######################################################################

add_executable(led_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/led_tests.cpp
)

gtest_discover_tests(led_tests)

target_include_directories(led_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(led_tests
    core
    GTest::gtest
)

set_target_properties(led_tests
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)