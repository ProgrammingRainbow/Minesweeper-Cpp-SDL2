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

void Game::reset() { this->board->reset(); }

void Game::init() {
    this->rows = 9;
    this->columns = 9;
    this->scale = 2;
    this->mine_count = 8;

    this->initSdl();

    this->border.reset(
        new Border(this->renderer, this->rows, this->columns, this->scale));
    this->border->init();

    this->board.reset(new Board(this->renderer, this->rows, this->columns,
                                this->scale, this->mine_count));
    this->board->init();

    this->mines.reset(new Mines(this->renderer, this->scale));
    this->mines->init();

    this->clock.reset(new Clock(this->renderer, this->columns, this->scale));
    this->clock->init();

    this->face.reset(new Face(this->renderer, this->columns, this->scale));
    this->face->init();
}

void Game::setScale() {
    this->border->setScale(this->scale);
    this->board->setScale(this->scale);
    this->mines->setScale(this->scale);
    this->clock->setScale(this->scale);
    this->face->setScale(this->scale);

    int window_width = (PIECE_SIZE * (static_cast<int>(this->columns) + 1) -
                        BORDER_LEFT + BORDER_RIGHT) *
                       this->scale;
    int window_height = (PIECE_SIZE * (static_cast<int>(this->rows)) +
                         BORDER_HEIGHT + BORDER_BOTTOM) *
                        this->scale;
    SDL_SetWindowSize(this->window.get(), window_width, window_height);
    SDL_SetWindowPosition(this->window.get(), SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
}

void Game::toggleScale() {
    this->scale = (this->scale == 1) ? 2 : (this->scale == 2) ? 3 : 1;
    this->setScale();
}

void Game::mouseDown(int x, int y, Uint8 button) {
    if (button == SDL_BUTTON_LEFT) {
        this->face->mouseClick(x, y, true);
    }

    this->board->mouseDown(x, y, button);
    if (this->board->getPressed()) {
        this->face->questionFace();
    }
}

void Game::mouseUp(int x, int y, Uint8 button) {
    if (button == SDL_BUTTON_LEFT) {
        if (this->face->mouseClick(x, y, false)) {
            this->reset();
        }
    }

    this->board->mouseUp(x, y, button);
    this->face->defaultFace();
}

void Game::events() {
    while (SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
        case SDL_QUIT:
            this->is_running = false;
            return;
        case SDL_MOUSEBUTTONDOWN:
            this->mouseDown(this->event.button.x, this->event.button.y,
                            this->event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            this->mouseUp(this->event.button.x, this->event.button.y,
                          this->event.button.button);
            break;
        case SDL_KEYDOWN:
            switch (this->event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                this->is_running = false;
                return;
            case SDL_SCANCODE_B:
                this->toggleScale();
                break;
            case SDL_SCANCODE_N:
                this->reset();
                break;
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
