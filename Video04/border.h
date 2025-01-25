#ifndef BORDER_H
#define BORDER_H

#include "main.h"

class Border {
    public:
        Border(std::shared_ptr<SDL_Renderer> new_renderer, std::size_t new_rows,
               std::size_t new_columns)
            : renderer{new_renderer},
              image{nullptr, SDL_DestroyTexture},
              src_rects{},
              rows{new_rows},
              columns{new_columns},
              piece_height{},
              piece_width{},
              left_offset{} {}

        void init();
        void draw() const;

    private:
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> image;
        std::vector<SDL_Rect> src_rects;
        std::size_t rows;
        std::size_t columns;
        int piece_height;
        int piece_width;
        int left_offset;
};

#endif
