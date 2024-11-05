cmake_minimum_required(VERSION 3.25)
project(EmberEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set the compiler
if(APPLE)
    
endif()

if(WIN32)
    message(FATAL_ERROR "Windows is not supported yet.")
endif()

if(UNIX AND NOT APPLE)
    message(FATAL_ERROR "Linux is not supported yet.")
endif()

# Add the subdirectories
add_subdirectory(framework)
add_subdirectory(sandbox)