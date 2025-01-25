#ifndef LOAD_MEDIA_H
#define LOAD_MEDIA_H

#include "main.h"

std::vector<SDL_Rect> createRects(
    const std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> &texture,
    int width, int height);

#endif
