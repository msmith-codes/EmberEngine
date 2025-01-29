cmake_minimum_required(VERSION 3.25)
project(SandboxApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "src/*.cpp")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC EmberEngineFramework)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC "-static-libgcc")
    target_link_libraries(${PROJECT_NAME} PUBLIC "-static-libstdc++")
    target_link_libraries(${PROJECT_NAME} PUBLIC "-Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") 
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE_MODE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSET_PATH="./assets/")
endif()