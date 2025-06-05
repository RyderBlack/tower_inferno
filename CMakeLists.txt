cmake_minimum_required(VERSION 3.15)
project(TowerInferno VERSION 1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(PLATFORM_DESKTOP "Build for desktop" ON)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Download and build raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)

# Set raylib build options
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

# Download and populate raylib
FetchContent_MakeAvailable(raylib)

# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
    # Model
    src/model/GameModel.cpp
    src/model/Enemy.cpp
    src/model/Tower.cpp
    src/model/Projectile.cpp
    src/model/WaveManager.cpp
    # View
    src/view/GameView.cpp
    src/view/MenuView.cpp
    # Controller
    src/controller/GameController.cpp
    # Utils
    src/utils/ResourceManager.cpp
    src/utils/Utils.cpp
    src/utils/Observable.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/model
    ${CMAKE_CURRENT_SOURCE_DIR}/src/view
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets..."
)

# Set Windows subsystem to hide console window (Windows only)
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Enable C++17 for raylib
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Set compiler flags for Windows
if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wno-missing-braces)
    target_link_libraries(${PROJECT_NAME} PRIVATE -lopengl32 -lgdi32 -lwinmm -mwindows)
endif()
