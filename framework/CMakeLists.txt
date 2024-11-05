cmake_minimum_required(VERSION 3.25)
project(EmberEngine_Framework)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Get the sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Fetch Dependencies
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.4"
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "1.0.1"
)

set(GLAD_SOURCES "vendor/glad/glad.c")

FetchContent_MakeAvailable(glfw glm)

# Add the library
add_library(EmberEngine_Framework STATIC ${GLAD_SOURCES} ${SOURCES})

# Include the directories
target_include_directories(EmberEngine_Framework PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Link the libraries
target_link_libraries(${PROJECT_NAME} glfw glm)

# Link the platform specific libraries
if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
