add_executable(math_test
    ${CMAKE_CURRENT_SOURCE_DIR}/math_test.cpp
)

target_include_directories(math_test SYSTEM PUBLIC
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(math_test thermistor ${GTEST_LIBRARIES} pthread)
