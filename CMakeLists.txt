cmake_minimum_required(VERSION 4.0)
project(Ember)

#####################
#  Configure C/C++  #
#####################
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if(DEFINED EE_PLATFORM_DEFINE)
    add_compile_definitions(${EE_PLATFORM_DEFINE})
endif()

#####################
#  Configure CMake  #
#####################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

#####################
#  Compile Vendors  #
#####################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
)

FetchContent_MakeAvailable(raylib)

#########################
#  Compile EmberEngine  #
#########################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)
