#ifndef BOARD_H
#define BOARD_H

#include "main.h"

struct Position {
        int row;
        int column;
};

class Board {
    public:
        Board(std::shared_ptr<SDL_Renderer> new_renderer, std::size_t new_rows,
              std::size_t new_columns, int new_scale, int new_count)
            : renderer{new_renderer},
              image{nullptr, SDL_DestroyTexture},
              front_vec{},
              back_vec{},
              src_rects{},
              rect{},
              rows{new_rows},
              columns{new_columns},
              piece_size{},
              scale{new_scale},
              mine_count{new_count},
              rd{},
              gen{rd()},
              pos_stack{},
              pressed{},
              mines_marked{},
              game_status{},
              first_turn{true} {}

        void init();
        void reset();
        inline int gameStatus() const { return this->game_status; }
        inline int minesMarked() const { return this->mines_marked; }
        inline bool getPressed() const { return this->pressed; }
        void mouseDown(int x, int y, Uint8 button);
        void mouseUp(int x, int y, Uint8 button);
        void setScale(int new_scale);
        void draw() const;

    private:
        void uncover();
        void reveal();
        void checkWon();

        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> image;
        std::vector<std::vector<std::size_t>> front_vec;
        std::vector<std::vector<std::size_t>> back_vec;
        std::vector<SDL_Rect> src_rects;
        SDL_Rect rect;
        std::size_t rows;
        std::size_t columns;
        int piece_size;
        int scale;
        int mine_count;
        std::random_device rd;
        std::mt19937 gen;
        std::stack<Position> pos_stack;
        bool pressed;
        int mines_marked;
        int game_status;
        bool first_turn;
};

#endif
