cmake_minimum_required(VERSION 3.13.5)

project(OpenDesignRenderer LANGUAGES C CXX)
set(TEST_NAME "${PROJECT_NAME}_Test")

# Google Test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Define a value for testing images directory
add_definitions(-DTESTING_IMAGES_DIR="${CMAKE_SOURCE_DIR}/test/image-files/")

# Project source files
set(PROJECT_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/Image.cpp
    ${CMAKE_SOURCE_DIR}/src/ImageDimensions.cpp
    ${CMAKE_SOURCE_DIR}/src/PixelColor.cpp
    ${CMAKE_SOURCE_DIR}/src/PixelCoordinates.cpp
    ${CMAKE_SOURCE_DIR}/src/RenderingEngine.cpp
)

# Test source files
set(TEST_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/test/main.cpp
    ${CMAKE_SOURCE_DIR}/test/ImageTests.cpp
    ${CMAKE_SOURCE_DIR}/test/RenderingEngineTests.cpp
)

# Test libraries
set(TEST_LIBRARIES ${GTEST_BOTH_LIBRARIES})

# Add executables
add_executable(${TEST_NAME}
    ${PROJECT_SOURCE_FILES}
    ${TEST_SOURCE_FILES})
target_link_libraries(${TEST_NAME} ${TEST_LIBRARIES})

target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/)

add_test(${TEST_NAME} ${TEST_NAME})

add_definitions(
    -D_USE_MATH_DEFINES)

set_target_properties(${TEST_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO)

target_compile_options(${TEST_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4 # Warning level 4
    /WX # Treat warnings as errors
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
    -Wall
    -Wextra
    -Werror # Treat warnings as errors
    -Wshadow
    -pedantic
    >)
