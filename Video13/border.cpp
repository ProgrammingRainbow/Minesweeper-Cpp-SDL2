#include "border.h"
#include "load_media.h"

void Border::init() {
    this->image.reset(
        IMG_LoadTexture(this->renderer.get(), "images/borders.png"));
    if (!this->image) {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }

    this->src_rects = createRects(this->image, PIECE_SIZE, BORDER_HEIGHT);

    this->setScale(this->scale);
}

void Border::setScale(int new_scale) {
    this->scale = new_scale;

    this->piece_height = BORDER_HEIGHT * this->scale;
    this->piece_width = PIECE_SIZE * this->scale;
    this->left_offset = BORDER_LEFT * this->scale;
}

void Border::setTheme(std::size_t new_theme) { this->theme = new_theme * 8; }

void Border::setSize(std::size_t new_rows, std::size_t new_columns) {
    this->rows = new_rows;
    this->columns = new_columns;
}

void Border::draw() const {
    SDL_Rect dest_rect = {0, 0, this->piece_width, this->piece_height};
    dest_rect.x = -this->left_offset;
    dest_rect.y = 0;
    SDL_RenderCopy(this->renderer.get(), this->image.get(),
                   &this->src_rects[this->theme], &dest_rect);

    dest_rect.x = this->piece_width * (static_cast<int>(this->columns) + 1) -
                  this->left_offset;
    dest_rect.y = 0;
    SDL_RenderCopy(this->renderer.get(), this->image.get(),
                   &this->src_rects[2 + this->theme], &dest_rect);

    dest_rect.x = -this->left_offset;
    dest_rect.y =
        this->piece_width * static_cast<int>(this->rows) + this->piece_height;
    SDL_RenderCopy(this->renderer.get(), this->image.get(),
                   &this->src_rects[5 + this->theme], &dest_rect);

    dest_rect.x = this->piece_width * (static_cast<int>(this->columns) + 1) -
                  this->left_offset;
    dest_rect.y =
        this->piece_width * static_cast<int>(this->rows) + this->piece_height;
    SDL_RenderCopy(this->renderer.get(), this->image.get(),
                   &this->src_rects[7 + this->theme], &dest_rect);

    for (int r = 0; r < static_cast<int>(this->rows); r++) {
        dest_rect.x = -this->left_offset;
        dest_rect.y = r * this->piece_width + this->piece_height;
        SDL_RenderCopy(this->renderer.get(), this->image.get(),
                       &this->src_rects[3 + this->theme], &dest_rect);

        dest_rect.x =
            this->piece_width * (static_cast<int>(this->columns) + 1) -
            this->left_offset;
        dest_rect.y = r * this->piece_width + this->piece_height;
        SDL_RenderCopy(this->renderer.get(), this->image.get(),
                       &this->src_rects[4 + this->theme], &dest_rect);
    }

    for (int c = 0; c < static_cast<int>(this->columns); c++) {
        dest_rect.x = (c + 1) * this->piece_width - this->left_offset;
        dest_rect.y = 0;
        SDL_RenderCopy(this->renderer.get(), this->image.get(),
                       &this->src_rects[1 + this->theme], &dest_rect);

        dest_rect.x = (c + 1) * this->piece_width - this->left_offset;
        dest_rect.y = this->piece_width * static_cast<int>(this->rows) +
                      this->piece_height;
        SDL_RenderCopy(this->renderer.get(), this->image.get(),
                       &this->src_rects[6 + this->theme], &dest_rect);
    }
}
