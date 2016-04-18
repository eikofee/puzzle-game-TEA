cmake_minimum_required(VERSION 2.6)

project(Rush-Hour)

#Définitions des flags de gcc
set(CMAKE_C_FLAGS "-std=c99 -g -Wall ")


#Définition de la localisation des librairies après la création de leurs archives (*.a)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


#Dossier des headers
include_directories (${CMAKE_BINARY_DIR}/include)


#Dossier des fichiers sources à compiler
add_subdirectory(./src/)

#Cibles pour les exécutables rh (rush hour) et ar (ane rouge)
ADD_CUSTOM_TARGET(rush)
ADD_DEPENDENCIES(rush rush-hour)

ADD_CUSTOM_TARGET(anerouge)
ADD_DEPENDENCIES(anerouge klotski)

#préparation des fichiers de test
add_executable(test_piece1 src/test/test_piece1)
add_executable(game_test src/test/game-test)
target_link_libraries(game_test game )
target_link_libraries(test_piece1 game )

#tests
enable_testing()
add_test(piece ${CMAKE_BINARY_DIR}/test_piece1)
add_test(game ${CMAKE_BINARY_DIR}/game_test)

#test_piece1.c
set_tests_properties(piece PROPERTIES PASS_REGULAR_EXPRESSION "Youpi !")

#game-test.c
set_tests_properties(game PROPERTIES PASS_REGULAR_EXPRESSION "Good.")


