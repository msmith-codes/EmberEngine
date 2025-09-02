cmake_minimum_required(VERSION 4.0)
project(Asteroids)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
        EmberEngine
)
