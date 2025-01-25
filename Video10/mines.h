#ifndef MINES_H
#define MINES_H

#include "main.h"

class Mines {
    public:
        Mines(std::shared_ptr<SDL_Renderer> new_renderer, int new_scale,
              int new_count)
            : renderer{new_renderer},
              back_image{nullptr, SDL_DestroyTexture},
              digit_image{nullptr, SDL_DestroyTexture},
              back_src_rects{},
              digit_src_rects{},
              back_dest_rect{},
              digit_rect{},
              scale{new_scale},
              mine_count{new_count},
              digits(3, 0) {}

        void init();
        void reset(int new_count);
        void increment();
        void decrement();
        void setScale(int new_scale);
        void draw() const;

    private:
        void updateDigits();

        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> back_image;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> digit_image;
        std::vector<SDL_Rect> back_src_rects;
        std::vector<SDL_Rect> digit_src_rects;
        SDL_Rect back_dest_rect;
        SDL_Rect digit_rect;
        int scale;
        int mine_count;
        std::vector<std::size_t> digits;
};

#endif
