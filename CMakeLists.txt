cmake_minimum_required(VERSION 3.10)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pour éviter les avertissements MSYS
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# Chemins de SFML (MSYS2)
set(SFML_DIR "C:/msys64/mingw64/lib/cmake/SFML")

# Trouver les packages SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Fichier source
add_executable(app main.cpp)

# Lier SFML au projet
target_link_libraries(app sfml-graphics sfml-window sfml-system)
