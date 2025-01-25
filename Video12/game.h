#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "border.h"
#include "board.h"
#include "mines.h"
#include "clock.h"
#include "face.h"

class Game {
    public:
        Game()
            : window{nullptr, SDL_DestroyWindow},
              renderer{nullptr, SDL_DestroyRenderer},
              border{nullptr},
              board{nullptr},
              mines{nullptr},
              clock{nullptr},
              face{nullptr},
              rows{},
              columns{},
              scale{},
              mine_count{},
              event{},
              is_running{true},
              is_playing{true},
              difficulty{} {}
        ~Game();

        void init();
        void run();

    private:
        void initSdl();
        void reset();
        void setScale();
        void toggleScale();
        void setTheme(std::size_t theme);
        void setDifficulty(double new_diff);
        void mouseDown(int x, int y, Uint8 button);
        void mouseUp(int x, int y, Uint8 button);
        void events();
        void update();
        void draw() const;

        std::shared_ptr<SDL_Window> window;
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<Border> border;
        std::unique_ptr<Board> board;
        std::unique_ptr<Mines> mines;
        std::unique_ptr<Clock> clock;
        std::unique_ptr<Face> face;
        std::size_t rows;
        std::size_t columns;
        int scale;
        int mine_count;
        SDL_Event event;
        bool is_running;
        bool is_playing;
        double difficulty;
};

#endif
