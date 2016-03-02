#Définit le répertoire d'installation du programme (-> /bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Crée un fichier games.txt qui se trouve dans /bin/games.txt. Le programme sera donc a exécuter depuis /bin
# On lui ajoute les seed des niveaux du jeu
file(WRITE ${CMAKE_BINARY_DIR}/bin/games.txt "4103300222052\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games.txt "8103022024034144301040250\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games.txt "8123024012040042145250301\n")


# On définit les éléments nécessaires à libgame.a puis on la crée
add_library(game game.c piece.c interface_txt)


# On crée l'exécutable rush-hour à partir du fichier rush-hour.c 
add_executable(rush-hour rush-hour)


# On lie les librairies dont rush-hour.c a besoin (game)
target_link_libraries(rush-hour game)
