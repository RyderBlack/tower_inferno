cmake_minimum_required(VERSION 3.15)
project(tower_inferno)

# Set Windows subsystem to GUI to avoid console window
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    add_definitions(-DSFML_STATIC)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SFML path
set(SFML_DIR "C:/msys64/mingw64/lib/cmake/SFML" CACHE PATH "SFML directory")

# Find SFML 2.6.1
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.6.1 COMPONENTS graphics window system audio REQUIRED)

# Set SFML_STATIC if needed
if(SFML_STATIC_LIBRARIES)
    add_definitions(-DSFML_STATIC)
endif()

# Output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set source files
set(SOURCES
    Tower_defense/main.cpp
    Tower_defense/monster.cpp
    Tower_defense/tower.cpp
)

# Create the executable
add_executable(tower_inferno WIN32 ${SOURCES})

# Include directories
target_include_directories(tower_inferno PRIVATE 
    "C:/msys64/mingw64/include"
    ${CMAKE_SOURCE_DIR}/Tower_defense
)

# Link SFML libraries
target_link_libraries(tower_inferno 
    sfml-graphics 
    sfml-window 
    sfml-system
    sfml-audio
)

# On Windows, we need to link against winmm, gdi32, and opengl32 for SFML
if(WIN32)
    target_link_libraries(tower_inferno 
        winmm 
        gdi32 
        opengl32
    )
endif()

# Copy DLLs on Windows
if (WIN32)
    # Set the path to the SFML bin directory
    set(SFML_BIN_DIR "C:/msys64/mingw64/bin")
    
    add_custom_command(TARGET tower_inferno POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_BIN_DIR}/libsfml-graphics-2.dll
            ${SFML_BIN_DIR}/libsfml-window-2.dll
            ${SFML_BIN_DIR}/libsfml-system-2.dll
            ${SFML_BIN_DIR}/libsfml-audio-2.dll
            $<TARGET_FILE_DIR:tower_inferno>
        COMMENT "Copying SFML DLLs..."
    )
    
    # Copy assets
    file(GLOB_RECURSE ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
    add_custom_command(TARGET tower_inferno POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:tower_inferno>/assets
        COMMENT "Copying assets..."
    )
endif()