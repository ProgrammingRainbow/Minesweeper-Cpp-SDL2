#include "clock.h"
#include "load_media.h"

void Clock::init() {
    this->back_dest_rect.x =
        (PIECE_SIZE * (static_cast<int>(this->columns) + 1) - BORDER_LEFT -
         DIGIT_BACK_WIDTH - DIGIT_BACK_RIGHT) *
        2;
    this->back_dest_rect.y = DIGIT_BACK_TOP * 2;
    this->back_dest_rect.w = DIGIT_BACK_WIDTH * 2;
    this->back_dest_rect.h = DIGIT_BACK_HEIGHT * 2;
    this->digit_rect.x = this->back_dest_rect.x + 2;
    this->digit_rect.y = DIGIT_BACK_TOP * 2 + 2;
    this->digit_rect.w = DIGIT_WIDTH * 2;
    this->digit_rect.h = DIGIT_HEIGHT * 2;

    this->back_image.reset(
        IMG_LoadTexture(this->renderer.get(), "images/digitback.png"));
    if (!this->back_image) {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }

    this->digit_image.reset(
        IMG_LoadTexture(this->renderer.get(), "images/digits.png"));
    if (!this->digit_image) {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }

    this->back_src_rects =
        createRects(this->back_image, DIGIT_BACK_WIDTH, DIGIT_BACK_HEIGHT);

    this->digit_src_rects =
        createRects(this->digit_image, DIGIT_WIDTH, DIGIT_HEIGHT);
}

void Clock::draw() const {
    SDL_RenderCopy(this->renderer.get(), this->back_image.get(),
                   &this->back_src_rects[0], &this->back_dest_rect);

    SDL_Rect dest_rect = this->digit_rect;
    for (int i = 0; i < 3; i++) {
        dest_rect.x = this->digit_rect.x + this->digit_rect.w * i;
        SDL_RenderCopy(this->renderer.get(), this->digit_image.get(),
                       &this->digit_src_rects[0], &dest_rect);
    }
}
