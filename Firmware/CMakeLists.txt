cmake_minimum_required(VERSION 3.20)
project(NanoThermostat C CXX)

# Allows to set the Multithreaded Debug Lib on Windows builds
cmake_policy(SET CMP0091 NEW)

include(GoogleTest)
if(WIN32)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
else()
    find_package(GTest REQUIRED)
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
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
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Core
    ${CMAKE_CURRENT_BINARY_DIR}/Core
)

# Does not compile with the same architecture -> needs to use the AVR toolchain
if(0)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Hal
        ${CMAKE_CURRENT_BINARY_DIR}/Hal
    )
endif()