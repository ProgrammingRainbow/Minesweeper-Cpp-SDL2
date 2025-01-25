#include "clock.h"
#include "load_media.h"

void Clock::init() {
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

void Clock::updateDigits() {
    std::size_t secs = (this->seconds > DIGIT_MAX) ? DIGIT_MAX : this->seconds;

    this->digits[0] = (secs > 99) ? (secs / 100) : DIGIT_DEFAULT;
    this->digits[1] = (secs > 9) ? ((secs / 10) % 10) : DIGIT_DEFAULT;
    this->digits[2] = secs % 10;
}

void Clock::reset() {
    this->last_time = SDL_GetTicks();
    this->seconds = 0;
    this->updateDigits();
}

void Clock::setScale(int new_scale) {
    this->scale = new_scale;

    this->back_dest_rect.x =
        (PIECE_SIZE * (static_cast<int>(this->columns) + 1) - BORDER_LEFT -
         DIGIT_BACK_WIDTH - DIGIT_BACK_RIGHT) *
        this->scale;
    this->back_dest_rect.y = DIGIT_BACK_TOP * this->scale;
    this->back_dest_rect.w = DIGIT_BACK_WIDTH * this->scale;
    this->back_dest_rect.h = DIGIT_BACK_HEIGHT * this->scale;
    this->digit_rect.x = this->back_dest_rect.x + this->scale;
    this->digit_rect.y = DIGIT_BACK_TOP * this->scale + this->scale;
    this->digit_rect.w = DIGIT_WIDTH * this->scale;
    this->digit_rect.h = DIGIT_HEIGHT * this->scale;
}

void Clock::setTheme(std::size_t new_theme) {
    this->back_theme = new_theme;
    this->digit_theme = new_theme * 12;
}

void Clock::setSize(std::size_t new_columns) {
    this->columns = new_columns;

    this->back_dest_rect.x =
        (PIECE_SIZE * (static_cast<int>(this->columns) + 1) - BORDER_LEFT -
         DIGIT_BACK_WIDTH - DIGIT_BACK_RIGHT) *
        this->scale;
    this->digit_rect.x = this->back_dest_rect.x + this->scale;
}

void Clock::update() {
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = 0;

    if (current_time >= this->last_time) {
        elapsed_time = current_time - this->last_time;
    } else {
        elapsed_time = (Uint32)-1 - this->last_time + current_time;
    }

    if (elapsed_time > 1000) {
        this->last_time += 1000;
        this->seconds++;
        this->updateDigits();
    }
}

void Clock::draw() const {
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
