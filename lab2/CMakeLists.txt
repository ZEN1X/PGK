cmake_minimum_required(VERSION 3.10)

# Project Name and Languages Used
project(lab2 VERSION 1.0 LANGUAGES CXX)

# Prevent for in-source building
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

# Specify the C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Set the warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Include directory for header files
set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

# Automatically find source files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Find SFML, specifying the version requirement
set(SFML_DIR "/usr/local/Cellar/sfml@2/2.6.2_1/lib/cmake/SFML") # Temporary fix to use SFML 2, while SFML 3 has already released
find_package(SFML 2.6.2 COMPONENTS graphics window system REQUIRED)

# Add the executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# Link the executable with the SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
