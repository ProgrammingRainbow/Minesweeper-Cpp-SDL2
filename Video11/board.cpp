#include "board.h"
#include "load_media.h"

void Board::init() {
    this->image.reset(
        IMG_LoadTexture(this->renderer.get(), "images/board.png"));
    if (!this->image) {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }

    this->src_rects = createRects(this->image, PIECE_SIZE, PIECE_SIZE);

    this->setScale(this->scale);

    this->reset(true);
}

void Board::reset(bool full_reset) {
    if (full_reset) {
        this->front_vec.resize(this->rows,
                               std::vector<std::size_t>(this->columns));
        for (auto &row : this->front_vec) {
            row.assign(this->columns, 9);
        }
    } else {
        for (auto &row : this->front_vec) {
            for (auto &elem : row) {
                elem = ((elem == 10) || (elem == 11)) ? elem : 9;
            }
        }
    }

    this->back_vec.resize(this->rows, std::vector<std::size_t>(this->columns));
    for (auto &row : this->back_vec) {
        row.assign(this->columns, 0);
    }

    std::uniform_int_distribution<std::size_t> rand_row(0, back_vec.size() - 1);
    std::uniform_int_distribution<std::size_t> rand_col(0,
                                                        back_vec[0].size() - 1);

    int add_mines = this->mine_count;
    while (add_mines > 0) {
        std::size_t r = rand_row(this->gen);
        std::size_t c = rand_col(this->gen);
        if (!this->back_vec[r][c]) {
            this->back_vec[r][c] = 13;
            add_mines--;
        }
    }

    for (int row = 0; row < static_cast<int>(back_vec.size()); row++) {
        auto un_row = static_cast<std::size_t>(row);
        for (int column = 0; column < static_cast<int>(back_vec[0].size());
             column++) {
            auto un_column = static_cast<std::size_t>(column);
            std::size_t close_mines = 0;
            if (this->back_vec[un_row][un_column] != 13) {
                for (int r = row - 1; r < row + 2; r++) {
                    if (r >= 0 && r < static_cast<int>(back_vec.size())) {
                        auto un_r = static_cast<std::size_t>(r);
                        for (int c = column - 1; c < column + 2; c++) {
                            if (c >= 0 &&
                                c < static_cast<int>(back_vec[0].size())) {
                                auto un_c = static_cast<std::size_t>(c);
                                if (this->back_vec[un_r][un_c] == 13) {
                                    close_mines++;
                                }
                            }
                        }
                    }
                }
                this->back_vec[un_row][un_column] = close_mines;
            }
        }
    }

    this->mines_marked = 0;
    this->game_status = 0;
    this->first_turn = true;
}

void Board::uncover() {
    while (!this->pos_stack.empty()) {
        Position pos = this->pos_stack.top();
        this->pos_stack.pop();

        for (int r = pos.row - 1; r < pos.row + 2; r++) {
            if (r >= 0 && r < static_cast<int>(back_vec.size())) {
                auto un_r = static_cast<std::size_t>(r);
                for (int c = pos.column - 1; c < pos.column + 2; c++) {
                    if (c >= 0 && c < static_cast<int>(back_vec[0].size())) {
                        auto un_c = static_cast<std::size_t>(c);
                        if (this->front_vec[un_r][un_c] == 9) {
                            this->front_vec[un_r][un_c] =
                                this->back_vec[un_r][un_c];
                            if (this->front_vec[un_r][un_c] == 0) {
                                this->pos_stack.push({r, c});
                            }
                        }
                    }
                }
            }
        }
    }
}

void Board::reveal() {
    for (std::size_t row = 0; row < this->front_vec.size(); row++) {
        for (std::size_t column = 0; column < this->front_vec[0].size();
             column++) {
            if (this->front_vec[row][column] == 9 &&
                this->back_vec[row][column] == 13) {
                this->front_vec[row][column] = 13;
            }
            if (this->front_vec[row][column] == 10 &&
                this->back_vec[row][column] != 13) {
                this->front_vec[row][column] = 15;
            }
        }
    }
}

void Board::checkWon() {
    for (std::size_t row = 0; row < this->front_vec.size(); row++) {
        for (std::size_t column = 0; column < this->front_vec[0].size();
             column++) {
            if (this->back_vec[row][column] != 13) {
                if (this->front_vec[row][column] !=
                    this->back_vec[row][column]) {
                    return;
                }
            }
        }
    }
    this->game_status = 1;
}

void Board::mouseDown(int x, int y, Uint8 button) {
    this->pressed = false;
    this->mines_marked = 0;

    if (x < this->rect.x || x > this->rect.x + this->rect.w) {
        return;
    }
    if (y < this->rect.y || y > this->rect.y + this->rect.h) {
        return;
    }

    auto row = static_cast<std::size_t>((y - this->rect.y) / this->piece_size);
    auto column =
        static_cast<std::size_t>((x - this->rect.x) / this->piece_size);

    if (button == SDL_BUTTON_LEFT) {
        if (this->front_vec[row][column] == 9) {
            this->pressed = true;
        }
    } else if (button == SDL_BUTTON_RIGHT) {
        if (this->front_vec[row][column] > 8 &&
            this->front_vec[row][column] < 12) {
            this->pressed = true;
        }
    }
}

void Board::mouseUp(int x, int y, Uint8 button) {
    if (!this->pressed) {
        return;
    }

    this->pressed = false;

    if (x < this->rect.x || x > this->rect.x + this->rect.w) {
        return;
    }
    if (y < this->rect.y || y > this->rect.y + this->rect.h) {
        return;
    }

    auto row = static_cast<std::size_t>((y - this->rect.y) / this->piece_size);
    auto column =
        static_cast<std::size_t>((x - this->rect.x) / this->piece_size);

    if (button == SDL_BUTTON_LEFT) {
        if (this->front_vec[row][column] == 9) {
            while (true) {
                if (this->back_vec[row][column] == 13) {
                    this->front_vec[row][column] = 14;
                    this->game_status = -1;
                } else {
                    this->front_vec[row][column] = this->back_vec[row][column];
                    if (this->front_vec[row][column] == 0) {
                        this->pos_stack.push(
                            {static_cast<int>(row), static_cast<int>(column)});
                        this->uncover();
                    }
                    this->checkWon();
                }
                if (this->first_turn && this->game_status != 0) {
                    this->reset(false);
                } else {
                    break;
                }
            }
            this->first_turn = false;

            if (this->game_status != 0) {
                this->reveal();
            }
        }
    }

    if (button == SDL_BUTTON_RIGHT) {
        if (this->front_vec[row][column] == 9) {
            this->front_vec[row][column]++;
            this->mines_marked = -1;
        } else if (this->front_vec[row][column] == 10) {
            this->front_vec[row][column]++;
            this->mines_marked = 1;
        } else if (this->front_vec[row][column] == 11) {
            this->front_vec[row][column] = 9;
        }
    }
}

void Board::setScale(int s) {
    this->scale = s;
    this->piece_size = PIECE_SIZE * this->scale;
    this->rect.x = (PIECE_SIZE - BORDER_LEFT) * this->scale;
    this->rect.y = BORDER_HEIGHT * this->scale;
    this->rect.w = PIECE_SIZE * static_cast<int>(this->columns) * this->scale;
    this->rect.h = BORDER_HEIGHT * static_cast<int>(this->rows) * this->scale;
}

void Board::draw() const {
    SDL_Rect dest_rect = {0, 0, this->piece_size, this->piece_size};
    for (std::size_t r = 0; r < this->front_vec.size(); r++) {
        dest_rect.y = static_cast<int>(r) * dest_rect.h + this->rect.y;
        for (std::size_t c = 0; c < this->front_vec[0].size(); c++) {
            dest_rect.x = static_cast<int>(c) * dest_rect.w + this->rect.x;
            std::size_t rect_index = this->front_vec[r][c];
            SDL_RenderCopy(this->renderer.get(), this->image.get(),
                           &this->src_rects[rect_index], &dest_rect);
        }
    }
}
