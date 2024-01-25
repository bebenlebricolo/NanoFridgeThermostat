add_executable(thermistor_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor_test.cpp
)

gtest_discover_tests(thermistor_tests)

target_include_directories(thermistor_tests SYSTEM PUBLIC
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(thermistor_tests
    thermistor
    ${GTEST_LIBRARIES}
)