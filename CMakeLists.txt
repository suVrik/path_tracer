cmake_minimum_required(VERSION 3.19)

project(softPATH)

find_package(SDL2 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")
add_executable(softPATH ${SOURCES})

target_link_libraries(softPATH PRIVATE SDL2::SDL2 SDL2::SDL2main)
