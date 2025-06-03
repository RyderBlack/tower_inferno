cmake_minimum_required(VERSION 3.10)
project(tower_inferno)

set(CMAKE_CXX_STANDARD 17)

# Add raylib
find_package(raylib 4.5 QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 4.5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Set include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/model
    ${CMAKE_SOURCE_DIR}/src/view
    ${CMAKE_SOURCE_DIR}/src/controller
)

# Source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/model/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/view/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/controller/*.cpp"
)

add_executable(tower_inferno ${SOURCES})
target_include_directories(tower_inferno PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tower_inferno PRIVATE raylib)

# For Windows: link winmm, gdi32, shell32 if needed
if (WIN32)
    target_link_libraries(tower_inferno PRIVATE winmm gdi32 shell32)
endif()

# Copy assets to build directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
file(COPY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
