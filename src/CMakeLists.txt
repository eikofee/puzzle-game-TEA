#tests
enable_testing()
add_test(piece test_piece1.c)
add_test(game game-test.c)
#test_piece1.c
set_test_properties(piece PROPERTIES PASS_REGULAR_EXPRESSION Youpi !\n")

#game-test.c
set_test_properties(game PROPERTIES PASS_REGULAR_EXPRESSION "> Préparation des tests:\n> création des pièces...\nDone.\n\n> new_game_hr...\nPièce [0] : x = 0, y = 3\nPièce [1] : x = 2, y = 2\nPièce [2] : x = 5, y = 2\nPièce [3] : x = 0, y = 0\nDone.\n\n> game_piece...\nValeur de x attendue : 0 || Valeur obtenue : 0\nValeur de y attendue : 3 || Valeur obtenue : 3\nValeur de isHorizontal attendue : 1 || Valeur obtenue : 1\nValeur de isSmall attendu : 1 || Valeur obtenue : 1\n\nValeur de x attendue : 2 || Valeur obtenue : 2\nValeur de y attendue : 2 || Valeur obtenue : 2\nValeur de isHorizontal attendue : 0 || Valeur obtenue : 0\nValeur de isSmall attendu : 0 || Valeur obtenue : 0\n\nValeur de x attendue : 5 || Valeur obtenue : 5\nValeur de y attendue : 2 || Valeur obtenue : 2\nValeur de isHorizontal attendue : 1 || Valeur obtenue : 1\nValeur de isSmall attendu : 0 || Valeur obtenue : 0\n\nValeur de x attendue : 0 || Valeur obtenue : 0\nValeur de y attendue : 0 || Valeur obtenue : 0\nValeur de isHorizontal attendue : 0 || Valeur obtenue : 0\nValeur de isSmall attendu : 1 || Valeur obtenue : 1\n\ngame_piece Done\n\n> création d'un jeu vide...\nDone.\n\n> copy_game...\nPieces src : \nPièce [0] : x = 0, y = 3\nPièce [1] : x = 2, y = 2\nPièce [2] : x = 5, y = 2\nPièce [3] : x = 0, y = 0\nPièces dst : \n Pièce [0] : x = 0, y = 3\nPièce [1] : x = 2, y = 2\nPièce [2] : x = 5, y = 2\nPièce [3] : x = 0, y = 0\n> delete_game(g2)...\nDone.\n\n> game_nb_piece...\nDone, attendu = 4, reçu = 4.\n\n> delete_game(g)...\nDone.\n\n")


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
