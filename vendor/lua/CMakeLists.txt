cmake_minimum_required(VERSION 4.0)
project(lua)

######################
#  Configure Target  #
######################
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

######################
#  Compile and Link  #
######################
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
