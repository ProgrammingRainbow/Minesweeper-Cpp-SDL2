#ifndef MINES_H
#define MINES_H

#include "main.h"

class Mines {
    public:
        Mines(std::shared_ptr<SDL_Renderer> new_renderer, int new_scale)
            : renderer{new_renderer},
              back_image{nullptr, SDL_DestroyTexture},
              digit_image{nullptr, SDL_DestroyTexture},
              back_src_rects{},
              digit_src_rects{},
              back_dest_rect{},
              digit_rect{},
              scale{new_scale} {}

        void init();
        void setScale(int new_scale);
        void draw() const;

    private:
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> back_image;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> digit_image;
        std::vector<SDL_Rect> back_src_rects;
        std::vector<SDL_Rect> digit_src_rects;
        SDL_Rect back_dest_rect;
        SDL_Rect digit_rect;
        int scale;
};

#endif
