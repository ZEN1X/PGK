cmake_minimum_required(VERSION 3.10)
project(lab4 VERSION 1.0 LANGUAGES CXX)

# Specify the C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify directories for header files and source files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find wxWidgets
# set(wxWidgets_CONFIG_EXECUTABLE /usr/local/bin/wx-config)
find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})

# Gather source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable based on the specified source files
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# Link against wxWidgets
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
