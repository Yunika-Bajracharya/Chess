cmake_minimum_required (VERSION 3.5)
set(PROJECT_NAME Chess)

project(Chess)

set (CMAKE_BUID_TYPE Debug)

set (source_dir "${PROJECT_SOURCE_DIR}/src/")
file (GLOB source_files "${source_dir}/*.cpp")

add_executable(${PROJECT_NAME} ${source_files})
if(UNIX) 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g -std=c++14 -lSDL2 -lGL -lSDL2_image -lSDL2_mixer")

elseif(WIN32)
    # for the vs compiler
    find_package(SDL2 PATHS . REQUIRED)
    include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES}) 
endif()

