#Definit le repertoire d'installation du programme (-> /bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Cree un fichier games.txt qui se trouve dans /bin/games.txt. Le programme sera donc a executer depuis /bin
# On lui ajoute les seed des niveaux du jeu
file(WRITE ${CMAKE_BINARY_DIR}/bin/games.txt "4103300222052\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games.txt "8103022024034144301040250\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games.txt "8123024012040042145250301\n")


# On definit les elements necessaires aux librairies (*.a) puis on les cree
#Librairie racine: game
add_library(game ${CMAKE_BINARY_DIR}/src/game/game.c ${CMAKE_BINARY_DIR}/src/piece/piece.c)
#Librairie spécifique à rush hour
add_library(rh ${CMAKE_BINARY_DIR}/src/interface/function_interface.c ${CMAKE_BINARY_DIR}/src/interface/interface_txt.c ${CMAKE_BINARY_DIR}/src/game/game_rh.c ${CMAKE_BINARY_DIR}/src/piece/piece_rh.c )
#Librairie spécifique à Ane Rouge
add_library(ar ${CMAKE_BINARY_DIR}/src/game/game_ar.c ${CMAKE_BINARY_DIR}/src/interface/function_interface.c ${CMAKE_BINARY_DIR}/src/interface/interface_txt.c)

# On cree l'executable x a partir du fichier correspondant... 
add_executable(rush-hour main/rush-hour)
add_executable(klotski main/klotski)


# On lie les librairies dont les exécutables ont besoin (game, rh/ar)
target_link_libraries(rush-hour game rh)
target_link_libraries(klotski game ar)