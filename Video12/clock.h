#ifndef CLOCK_H
#define CLOCK_H

#include "main.h"

class Clock {
    public:
        Clock(std::shared_ptr<SDL_Renderer> new_renderer,
              std::size_t new_columns, int new_scale)
            : renderer{new_renderer},
              back_image{nullptr, SDL_DestroyTexture},
              digit_image{nullptr, SDL_DestroyTexture},
              back_src_rects{},
              digit_src_rects{},
              back_dest_rect{},
              digit_rect{},
              columns{new_columns},
              scale{new_scale},
              digits(3, 0),
              seconds{},
              last_time{},
              back_theme{},
              digit_theme{} {}

        void init();
        void reset();
        void setScale(int new_scale);
        void setTheme(std::size_t new_theme);
        void update();
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
        std::size_t columns;
        int scale;
        std::vector<std::size_t> digits;
        std::size_t seconds;
        Uint32 last_time;
        std::size_t back_theme;
        std::size_t digit_theme;
};

#endif
