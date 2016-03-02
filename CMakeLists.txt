cmake_minimum_required(VERSION 2.6)

project(Rush-Hour)

#Définitions des flags de gcc
set(CMAKE_C_FLAGS "-std=c99 -g -w")


#Définition de la localisation des librairies après la création de leurs archives (*.a)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


#Dossier des headers
include_directories (src/)


#Dossier des fichiers sources à compiler
add_subdirectory(./src/)

