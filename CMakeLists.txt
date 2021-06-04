cmake_minimum_required(VERSION 3.19)

project(softPATH)

find_package(SDL2 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

file(GLOB_RECURSE HEADERS "source/*.h")
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(softPATH ${SOURCES} ${HEADERS})

target_link_libraries(softPATH PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(softPATH PRIVATE "source")

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/source"
    PREFIX "Header Files"
    FILES ${HEADERS}
)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/source"
    PREFIX "Source Files"
    FILES ${SOURCES}
)
