cmake_minimum_required(VERSION 3.22)
project(mentor)
set(CMAKE_CXX_STANDARD 20)

# ========================
# === External Libraries ===
# ========================
include(FetchContent)

# ==== Google Test =====
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ==== Boost =====


find_package(spdlog REQUIRED)
find_package(Boost REQUIRED)

# ========================
# === Project Settings ===
# ========================

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
)
add_definitions(-DLOGS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/logs/")
add_definitions(-DRESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/timer.h
        src/logger.h
        src/utils.h
        src/Thread.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# ========================
# ======= Testing ========
# ========================

enable_testing()

# Define the exercise executable name
set(TEST_EXECUTABLE_NAME ${PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES "exercises/*.cpp")

add_executable(
        ${TEST_EXECUTABLE_NAME}
        ${TEST_SOURCES}
)


# Link necessary libraries to the exercise executable
target_link_libraries(
        ${TEST_EXECUTABLE_NAME}
        GTest::gtest_main
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

# Include GoogleTest
include(GoogleTest)

# Discover and add tests to CTest
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
