#ifndef BOARD_H
#define BOARD_H

#include "main.h"

class Board {
    public:
        Board(std::shared_ptr<SDL_Renderer> new_renderer, std::size_t new_rows,
              std::size_t new_columns, int new_scale)
            : renderer{new_renderer},
              image{nullptr, SDL_DestroyTexture},
              front_vec{},
              src_rects{},
              rect{},
              rows{new_rows},
              columns{new_columns},
              piece_size{},
              scale{new_scale} {}

        void init();
        void setScale(int new_scale);
        void draw() const;

    private:
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> image;
        std::vector<std::vector<std::size_t>> front_vec;
        std::vector<SDL_Rect> src_rects;
        SDL_Rect rect;
        std::size_t rows;
        std::size_t columns;
        int piece_size;
        int scale;
};

#endif
