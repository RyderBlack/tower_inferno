cmake_minimum_required(VERSION 3.15)
project(tower_inferno)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration de SFML
set(SFML_DIR "C:/Users/ryrym_i6sf5hg/CLionProjects/Libraries/SFML-3.0.0/lib/cmake/SFML")

# Trouver SFML avec les composants nécessaires
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Afficher les informations de débogage
message(STATUS "SFML found: ${SFML_FOUND}")
message(STATUS "SFML version: ${SFML_VERSION_STRING}")
message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
message(STATUS "SFML include dir: ${SFML_INCLUDE_DIR}")

# Créer l'exécutable
add_executable(tower_inferno main.cpp)

# Lier les bibliothèques SFML nécessaires
target_link_libraries(tower_inferno PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Configuration pour copier les DLLs automatiquement
set(SFML_BIN_DIR "C:/Users/ryrym_i6sf5hg/CLionProjects/Libraries/SFML-3.0.0/bin")
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")

# Créer le répertoire de sortie s'il n'existe pas
file(MAKE_DIRECTORY ${OUTPUT_DIR})

# Copier les DLLs SFML nécessaires
add_custom_command(TARGET tower_inferno POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SFML_BIN_DIR}/sfml-graphics-3.dll"
    "${SFML_BIN_DIR}/sfml-window-3.dll"
    "${SFML_BIN_DIR}/sfml-system-3.dll"
    $<TARGET_FILE_DIR:tower_inferno>
    COMMENT "Copie des DLLs SFML..."
)

# Définir le répertoire de sortie des exécutables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
