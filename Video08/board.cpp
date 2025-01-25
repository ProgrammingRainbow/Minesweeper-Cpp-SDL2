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

    this->reset();
}

void Board::reset() {
    this->front_vec.resize(this->rows, std::vector<std::size_t>(this->columns));
    for (auto &row : this->front_vec) {
        row.assign(this->columns, 9);
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
}

void Board::setScale(int new_scale) {
    this->scale = new_scale;
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
