#CMakeLists.txt to build project using cmake
#run $cmake CMakeLists.txt

cmake_minimum_required(VERSION 2.6)

project(pgm)
include_directories("projet/src/")

#find_path(IRRLICHT_INCLUDE_DIR
#          "/usr/include/irrlicht")
 
#include_directories(${IRRLICHT_INCLUDE_DIR})
 include_directories("/usr/include/irrlicht")
 
find_library(IRRLICHT_LIBRARY
             irrlicht)

file(
GLOB_RECURSE
source_files
projet/src/*.[cht]pp
)

add_executable(pgm ${source_files})

SET(CMAKE_BUILD_TYPE Debug)
ADD_DEFINITIONS( -O2 -std=c++11 -Wno-comment  -Wno-unused-function)

TARGET_LINK_LIBRARIES(pgm -lIrrlicht)

#-Wall -Wextra 
