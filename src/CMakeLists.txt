#Definit le repertoire d'installation du programme (-> /bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Cree un fichier games.txt qui se trouve dans /bin/games.txt. Le programme sera donc a executer depuis /bin
# On lui ajoute les seed des niveaux du jeu
file(WRITE ${CMAKE_BINARY_DIR}/bin/games.txt "4103300222052\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games.txt "8103022024034144301040250\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games.txt "8123024012040042145250301\n")


# On definit les elements necessaires a libgame.a puis on la cree
add_library(game ${CMAKE_BINARY_DIR}/src/game/game.c ${CMAKE_BINARY_DIR}/src/piece/piece.c)
add_library(rh ${CMAKE_BINARY_DIR}/src/interface/function_interface.c ${CMAKE_BINARY_DIR}/src/interface/interface_txt.c ${CMAKE_BINARY_DIR}/src/game/game_rh.c ${CMAKE_BINARY_DIR}/src/piece/piece_rh.c )
add_library(ar ${CMAKE_BINARY_DIR}/src/game/game_ar.c ${CMAKE_BINARY_DIR}/src/interface/function_interface.c ${CMAKE_BINARY_DIR}/src/interface/interface_txt.c)
#On explicite que les headers se trouvent dans /lib/
#file(GLOB rh_HEADERS ${CMAKE_BINARY_DIR}/lib/function_interface.h ${CMAKE_BINARY_DIR}/lib/interface_txt.h ${CMAKE_BINARY_DIR}/lib/utility.h)

# On cree l'executable rush-hour a partir du fichier rush-hour.c 
add_executable(rush-hour main/rush-hour)
add_executable(klotski main/klotski)


# On lie les librairies dont rush-hour.c a besoin (game)
target_link_libraries(rush-hour game rh)
target_link_libraries(klotski game ar)