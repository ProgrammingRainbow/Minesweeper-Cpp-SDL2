#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <format>
#include <iostream>
#include <memory>

constexpr int SDL_FLAGS = (SDL_INIT_VIDEO | SDL_INIT_AUDIO);

constexpr const char *WINDOW_TITLE = "Minesweeper";
constexpr int WINDOW_WIDTH = 328;
constexpr int WINDOW_HEIGHT = 414;

#endif
