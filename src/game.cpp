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

void Game::setTitle() {
    std::string new_title = std::format("{} - {} - {}", WINDOW_TITLE,
                                        this->size_str, this->diff_str);

    SDL_SetWindowTitle(this->window.get(), new_title.c_str());
}

void Game::reset() {
    this->mine_count = static_cast<int>(
        static_cast<double>(this->rows * this->columns) * this->difficulty);

    this->board->reset(this->mine_count, true);
    this->mines->reset(this->mine_count);
    this->clock->reset();
    this->face->defaultFace();

    this->setTitle();

    this->is_playing = true;
}

void Game::init() {
    this->rows = 9;
    this->columns = 9;
    this->scale = 2;
    this->mine_count = 8;
    this->difficulty = 0.1;
    this->size_str = "Tiny";
    this->diff_str = "Easy";

    this->initSdl();

    this->border.reset(
        new Border(this->renderer, this->rows, this->columns, this->scale));
    this->border->init();

    this->board.reset(new Board(this->renderer, this->rows, this->columns,
                                this->scale, this->mine_count));
    this->board->init();

    this->mines.reset(new Mines(this->renderer, this->scale, this->mine_count));
    this->mines->init();

    this->clock.reset(new Clock(this->renderer, this->columns, this->scale));
    this->clock->init();

    this->face.reset(new Face(this->renderer, this->columns, this->scale));
    this->face->init();

    this->setTitle();
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

void Game::setTheme(std::size_t new_theme) {
    std::size_t binary_theme = (new_theme < 6) ? 0 : 1;
    std::size_t face_theme = (new_theme < 3) ? 0 : (new_theme < 6) ? 1 : 2;
    (void)binary_theme;
    (void)face_theme;

    this->board->setTheme(new_theme);
    this->border->setTheme(binary_theme);
    this->mines->setTheme(binary_theme);
    this->clock->setTheme(binary_theme);
    this->face->setTheme(face_theme);
}

void Game::setSize(std::size_t new_rows, std::size_t new_columns, int new_scale,
                   const std::string &new_size_str) {
    this->rows = new_rows;
    this->columns = new_columns;
    this->scale = new_scale;
    this->size_str = new_size_str;

    this->border->setSize(this->rows, this->columns);
    this->board->setSize(this->rows, this->columns);
    this->clock->setSize(this->columns);
    this->face->setSize(this->columns);

    this->setScale();

    this->reset();
}

void Game::setDifficulty(double new_diff, const std::string &new_diff_str) {
    this->difficulty = new_diff;
    this->diff_str = new_diff_str;

    this->reset();
}

void Game::mouseDown(int x, int y, Uint8 button) {
    if (button == SDL_BUTTON_LEFT) {
        this->face->mouseClick(x, y, true);
    }

    if (this->is_playing) {
        this->board->mouseDown(x, y, button);
        if (this->board->getPressed()) {
            this->face->questionFace();
        }
    }
}

void Game::mouseUp(int x, int y, Uint8 button) {
    if (button == SDL_BUTTON_LEFT) {
        if (this->face->mouseClick(x, y, false)) {
            this->reset();
        }
    }

    if (this->is_playing) {
        this->board->mouseUp(x, y, button);

        if (this->board->minesMarked() == 1) {
            this->mines->increment();
        } else if (this->board->minesMarked() == -1) {
            this->mines->decrement();
        }

        if (this->board->gameStatus() == 1) {
            this->face->wonFace();
            this->is_playing = false;
        } else if (this->board->gameStatus() == -1) {
            this->face->lostFace();
            this->is_playing = false;
        } else {
            this->face->defaultFace();
        }
    }
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
            case SDL_SCANCODE_1:
                this->setTheme(0);
                break;
            case SDL_SCANCODE_2:
                this->setTheme(1);
                break;
            case SDL_SCANCODE_3:
                this->setTheme(2);
                break;
            case SDL_SCANCODE_4:
                this->setTheme(3);
                break;
            case SDL_SCANCODE_5:
                this->setTheme(4);
                break;
            case SDL_SCANCODE_6:
                this->setTheme(5);
                break;
            case SDL_SCANCODE_7:
                this->setTheme(6);
                break;
            case SDL_SCANCODE_8:
                this->setTheme(7);
                break;
            case SDL_SCANCODE_A:
                this->setDifficulty(0.1, "Easy");
                break;
            case SDL_SCANCODE_S:
                this->setDifficulty(0.133, "Medium");
                break;
            case SDL_SCANCODE_D:
                this->setDifficulty(0.166, "Hard");
                break;
            case SDL_SCANCODE_F:
                this->setDifficulty(0.2, "Very Hard");
                break;
            case SDL_SCANCODE_Q:
                this->setSize(9, 9, 2, "Tiny");
                break;
            case SDL_SCANCODE_W:
                this->setSize(16, 16, 2, "Small");
                break;
            case SDL_SCANCODE_E:
                this->setSize(16, 30, 2, "Medium");
                break;
            case SDL_SCANCODE_R:
                this->setSize(20, 40, 2, "Large");
                break;
            case SDL_SCANCODE_T:
                this->setSize(40, 80, 1, "Huge");
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

void Game::update() {
    if (this->is_playing) {
        this->clock->update();
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

        this->update();

        this->draw();

        SDL_Delay(16);
    }
}
