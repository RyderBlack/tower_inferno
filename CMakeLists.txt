cmake_minimum_required(VERSION 3.15)
project(tower_inferno)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SFML path
set(SFML_DIR "C:/msys64/mingw64/lib/cmake/SFML" CACHE PATH "SFML directory")

# Find SFML 2.6.1
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)

# Output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create the executable
add_executable(tower_inferno 
    main.cpp
)

# Include SFML headers
target_include_directories(tower_inferno PRIVATE "C:/msys64/mingw64/include")

# Link SFML libraries
target_link_libraries(tower_inferno 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Copy DLLs on Windows
if (WIN32)
    # Set the path to the SFML bin directory
    set(SFML_BIN_DIR "C:/msys64/mingw64/bin")
    
    add_custom_command(TARGET tower_inferno POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_BIN_DIR}/libsfml-graphics-2.dll
            ${SFML_BIN_DIR}/libsfml-window-2.dll
            ${SFML_BIN_DIR}/libsfml-system-2.dll
            $<TARGET_FILE_DIR:tower_inferno>
        COMMENT "Copying SFML DLLs..."
    )
endif()
