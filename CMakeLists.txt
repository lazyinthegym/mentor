cmake_minimum_required(VERSION 3.23)
project(mentor)


# Download Gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(CMAKE_CXX_STANDARD 14)

add_definitions(-DROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/timer.h
        src/logger.h
        )


# ========================
# ======= Testing ========
# ========================

# Gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Define the test executable name
set(TEST_EXECUTABLE_NAME ${PROJECT_NAME}_test)

# Add the test executable target
add_executable(
        ${TEST_EXECUTABLE_NAME}
        test/hello_test.cpp
)

# Link necessary libraries to the test executable
target_link_libraries(
        ${TEST_EXECUTABLE_NAME}
        GTest::gtest_main
)

# Include GoogleTest
include(GoogleTest)

# Discover and add tests to CTest
gtest_discover_tests(${TEST_EXECUTABLE_NAME})