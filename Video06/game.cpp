#include "game.h"

Game::~Game() {
    this->face.reset();
    this->clock.reset();
    this->mines.reset();
    this->board.reset();
    this->border.reset();

    this->renderer.reset();
    this->window.reset();

    IMG_Quit();
    SDL_Quit();

    std::cout << "all clean!" << std::endl;
}

void Game::init() {
    this->rows = 9;
    this->columns = 9;

    this->initSdl();

    this->border.reset(new Border(this->renderer, this->rows, this->columns));
    this->border->init();

    this->board.reset(new Board(this->renderer, this->rows, this->columns));
    this->board->init();

    this->mines.reset(new Mines(this->renderer));
    this->mines->init();

    this->clock.reset(new Clock(this->renderer, this->columns));
    this->clock->init();

    this->face.reset(new Face(this->renderer, this->columns));
    this->face->init();
}

void Game::events() {
    while (SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
        case SDL_QUIT:
            this->is_running = false;
            return;
        case SDL_KEYDOWN:
            switch (this->event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                this->is_running = false;
                return;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void Game::draw() const {
    SDL_RenderClear(this->renderer.get());

    this->border->draw();
    this->board->draw();
    this->mines->draw();
    this->clock->draw();
    this->face->draw();

    SDL_RenderPresent(this->renderer.get());
}

void Game::run() {
    while (this->is_running) {
        this->events();

        this->draw();

        SDL_Delay(16);
    }
}
