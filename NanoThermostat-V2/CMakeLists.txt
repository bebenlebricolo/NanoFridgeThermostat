cmake_minimum_required(VERSION 3.8)
project(NanoThermostat C CXX)

include(GoogleTest)
if(WIN32)
    # Tells Visual Studio to use solution folders
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    add_library(GTest::GTest INTERFACE IMPORTED)
    target_link_libraries(GTest::GTest INTERFACE gtest_main)
else()
    find_package(GTest REQUIRED)
endif()
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -Wall -Wextra")
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -O0 -g3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -Wextra")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /Od /W4")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /Od /W4")
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /Od /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /W4 ")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /O2 /W4")
endif()

# Add subdirectories
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Math
    ${CMAKE_CURRENT_BINARY_DIR}/Math
)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thermistor
    ${CMAKE_CURRENT_BINARY_DIR}/Thermistor
)