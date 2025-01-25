#include "mines.h"
#include "load_media.h"

void Mines::init() {
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

    this->setScale(this->scale);

    this->updateDigits();
}

void Mines::updateDigits() {
    std::size_t mines = static_cast<std::size_t>(std::abs(this->mine_count));

    if (this->mine_count > DIGIT_MAX) {
        mines = DIGIT_MAX;
    }

    if (this->mine_count < -DIGIT_MIN) {
        mines = DIGIT_MIN;
    }

    this->digits[0] = (mines > 99) ? (mines / 100) : DIGIT_DEFAULT;
    this->digits[1] = (mines > 9) ? ((mines / 10) % 10) : DIGIT_DEFAULT;
    this->digits[2] = mines % 10;

    if (this->mine_count < 0) {
        this->digits[0] = 10;
    }
}

void Mines::reset(int new_count) {
    this->mine_count = new_count;
    this->updateDigits();
}

void Mines::increment() {
    this->mine_count++;
    this->updateDigits();
}

void Mines::decrement() {
    this->mine_count--;
    this->updateDigits();
}

void Mines::setScale(int new_scale) {
    this->scale = new_scale;

    this->back_dest_rect.x = DIGIT_BACK_LEFT * this->scale;
    this->back_dest_rect.y = DIGIT_BACK_TOP * this->scale;
    this->back_dest_rect.w = DIGIT_BACK_WIDTH * this->scale;
    this->back_dest_rect.h = DIGIT_BACK_HEIGHT * this->scale;
    this->digit_rect.x = DIGIT_BACK_LEFT * this->scale + this->scale;
    this->digit_rect.y = DIGIT_BACK_TOP * this->scale + this->scale;
    this->digit_rect.w = DIGIT_WIDTH * this->scale;
    this->digit_rect.h = DIGIT_HEIGHT * this->scale;
}

void Mines::setTheme(std::size_t new_theme) {
    this->back_theme = new_theme;
    this->digit_theme = new_theme * 12;
}

void Mines::draw() const {
    SDL_RenderCopy(this->renderer.get(), this->back_image.get(),
                   &this->back_src_rects[this->back_theme],
                   &this->back_dest_rect);

    SDL_Rect dest_rect = this->digit_rect;
    for (int i = 0; i < static_cast<int>(this->digits.size()); i++) {
        dest_rect.x = this->digit_rect.x + this->digit_rect.w * i;
        SDL_RenderCopy(
            this->renderer.get(), this->digit_image.get(),
            &this->digit_src_rects[this->digits[static_cast<std::size_t>(i)] +
                                   this->digit_theme],
            &dest_rect);
    }
}
