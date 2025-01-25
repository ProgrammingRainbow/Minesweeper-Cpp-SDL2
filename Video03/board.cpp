#include "board.h"
#include "load_media.h"

void Board::init() {
    this->rect.x = (PIECE_SIZE - BORDER_LEFT) * 2;
    this->rect.y = BORDER_HEIGHT * 2;
    this->rect.w = PIECE_SIZE * static_cast<int>(this->columns) * 2;
    this->rect.h = BORDER_HEIGHT * static_cast<int>(this->rows) * 2;
    this->piece_size = PIECE_SIZE * 2;

    this->image.reset(
        IMG_LoadTexture(this->renderer.get(), "images/board.png"));
    if (!this->image) {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }

    this->src_rects = createRects(this->image, PIECE_SIZE, PIECE_SIZE);

    this->front_vec.resize(this->rows,
                           std::vector<std::size_t>(this->columns, 9));
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
