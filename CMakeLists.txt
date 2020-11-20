cmake_minimum_required(VERSION 3.5)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -pedantic -Werror -Wold-style-cast")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Tetris main.cpp game.h game.cpp tetromino.h tetromino.cpp SDL_wrapper.h)

target_link_libraries(Tetris ${SDL2_LIBRARIES} SDL2_ttf)