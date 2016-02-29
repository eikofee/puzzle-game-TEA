set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_library(game game.c piece.c interface_txt)
add_executable(rush-hour rush-hour)
target_link_libraries(rush-hour game)

