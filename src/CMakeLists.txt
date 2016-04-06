#Definit le repertoire d'installation du programme (-> /bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Cree un fichier games_rh.txt qui se trouve dans /bin/games_rh.txt. Le programme sera donc a executer depuis /bin
# On lui ajoute les seed des niveaux du jeu
file(WRITE ${CMAKE_BINARY_DIR}/bin/games_rh.txt "4n6x6p2w2h1x0y3p2w3h1x0y0p1w1h3x2y2p1w1h2x5y2\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games_rh.txt "8n6x6p2w2h1x0y3p1w1h2x2y2p1w1h2x2y4p1w1h2x3y4p2w2h1x4y4p2w3h1x0y1p1w1h2x4y0p1w1h3x5y0\n")
file(APPEND ${CMAKE_BINARY_DIR}/bin/games_rh.txt "8n6x6p2w2h1x2y3p1w1h2x2y4p1w1h2x1y2p1w1h2x4y0p1w1h2x4y2p2w2h1x4y5p1w1h3x5y0p2w3h1x0y1\n")

# Cree un fichier games_ar.txt qui se trouve dans /bin/games_ar.txt. Le programme sera donc executer depuis 
# /bin. On lui ajout les seed des niveaux du jeu

file(WRITE ${CMAKE_BINARY_DIR}/bin/games_ar.txt "10n4x5p3w2h2x2y0p3w1h2x1y0p3w1h2x0y0p3w2h1x0y2p3w2h1x2y2p3w1h2x0y3p3w1h1x1y3p3w1h1x1y4p3w1h1x2y3p3w1h1x2y4\n")

file(APPEND ${CMAKE_BINARY_DIR}/bin/games_ar.txt "10n4x5p3w2h2x2y0p3w1h1x1y0p3w1h1x0y0p3w1h1x0y1p3w1h1x1y1p3w2h1x0y2p3w2h1x2y2p3w2h1x0y3p3w2h1x0y4p3w2h1x2y4\n")

file(APPEND ${CMAKE_BINARY_DIR}/bin/games_ar.txt "15n4x5p3w2h2x2y1p3w1h1x0y0p3w1h1x1y0p3w1h1x2y0p3w1h1x3y0p3w1h1x0y1p3w1h1x1y1p3w1h1x0y2p3w1h1x1y2p3w1h1x0y3p3w1h1x0y4p3w1h1x2y3p3w1h1x3y3p3w1h1x2y4p3w1h1x3y4\n")


file(WRITE ${CMAKE_BINARY_DIR}/config.ini "#Contiendra le nom du jeu exécuté")


# On definit les elements necessaires aux librairies (*.a) puis on les cree
#Librairie racine: game
add_library(game ${CMAKE_BINARY_DIR}/src/game/game.c ${CMAKE_BINARY_DIR}/src/piece/piece.c ${CMAKE_BINARY_DIR}/src/game/game_ar.c ${CMAKE_BINARY_DIR}/src/game/game_rh.c ${CMAKE_BINARY_DIR}/src/piece/piece_rh.c)
add_library(interface ${CMAKE_BINARY_DIR}/src/interface/function_interface.c ${CMAKE_BINARY_DIR}/src/interface/interface_txt.c ${CMAKE_BINARY_DIR}/src/solver/solver.c)

# On cree l'executable x a partir du fichier correspondant... 
add_executable(rush-hour main/rush-hour)
add_executable(klotski main/klotski)


# On lie les librairies dont les exécutables ont besoin (game, rh/ar)
target_link_libraries(rush-hour game interface)
target_link_libraries(klotski game interface)