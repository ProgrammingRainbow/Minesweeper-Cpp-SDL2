#ifndef FACE_H
#define FACE_H

#include "main.h"

class Face {
    public:
        Face(std::shared_ptr<SDL_Renderer> new_renderer,
             std::size_t new_columns, int new_scale)
            : renderer{new_renderer},
              image{nullptr, SDL_DestroyTexture},
              src_rects{},
              dest_rect{},
              columns{new_columns},
              scale{new_scale},
              image_index{} {}

        void init();
        void setScale(int new_scale);
        bool mouseClick(int x, int y, bool down);
        inline void defaultFace() { this->image_index = 0; }
        void draw() const;

    private:
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> image;
        std::vector<SDL_Rect> src_rects;
        SDL_Rect dest_rect;
        std::size_t columns;
        int scale;
        std::size_t image_index;
};

#endif
