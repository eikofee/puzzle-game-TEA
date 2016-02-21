set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_library(game game.c piece.c)
add_executable(rush-hour interface_txt)
target_link_libraries(rush-hour game)

