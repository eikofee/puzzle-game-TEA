cmake_minimum_required(VERSION 2.6)

project(Rush-Hour)

#Définitions des flags de gcc
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")


#Définition de la localisation des librairies après la création de leurs archives (*.a)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


#Dossier des headers
include_directories (src/)


#Dossier des fichiers sources à compiler
add_subdirectory(./src/)

#préparation des fichiers de test
add_executable(test_piece1 src/test_piece1)
add_executable(game_test src/game-test)
target_link_libraries(game_test game)
target_link_libraries(test_piece1 game)
#tests
enable_testing()
add_test(piece ${CMAKE_BINARY_DIR}/test_piece1)
add_test(game ${CMAKE_BINARY_DIR}/game_test)
#test_piece1.c
set_tests_properties(piece PROPERTIES PASS_REGULAR_EXPRESSION "Youpi !")

#game-test.c
set_tests_properties(game PROPERTIES PASS_REGULAR_EXPRESSION "Good.")


