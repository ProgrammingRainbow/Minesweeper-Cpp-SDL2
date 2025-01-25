#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <iostream>
#include <memory>
#include <vector>

constexpr int SDL_FLAGS = (SDL_INIT_VIDEO | SDL_INIT_AUDIO);
constexpr int IMG_FLAGS = IMG_INIT_PNG;

constexpr const char *WINDOW_TITLE = "Minesweeper";
constexpr int WINDOW_WIDTH = 328;
constexpr int WINDOW_HEIGHT = 414;

constexpr int PIECE_SIZE = 16;
constexpr int BORDER_HEIGHT = 55;
constexpr int BORDER_LEFT = 4;
constexpr int BORDER_RIGHT = 8;
constexpr int BORDER_BOTTOM = 8;

#endif
