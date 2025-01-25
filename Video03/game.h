#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "border.h"
#include "board.h"

class Game {
    public:
        Game()
            : window{nullptr, SDL_DestroyWindow},
              renderer{nullptr, SDL_DestroyRenderer},
              border{nullptr},
              board{nullptr},
              rows{},
              columns{},
              event{},
              is_running{true} {}

        ~Game();

        void init();
        void run();

    private:
        void initSdl();
        void events();
        void update();
        void draw() const;

        std::shared_ptr<SDL_Window> window;
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<Border> border;
        std::unique_ptr<Board> board;
        std::size_t rows;
        std::size_t columns;
        SDL_Event event;
        bool is_running;
};

#endif
