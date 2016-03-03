#On définit les éléments nécessaires à libgame.a puis on la crée
add_library(game game.c piece.c interface_txt)



#Définit le répertoire d'installation du programme final(-> /bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# On crée l'exécutable rush-hour à partir du fichier rush-hour.c 
add_executable(rush-hour rush-hour)


# On lie les librairies dont rush-hour.c a besoin (game)
target_link_libraries(rush-hour game)
