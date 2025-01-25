#include "face.h"
#include "load_media.h"

void Face::init() {
    this->image.reset(
        IMG_LoadTexture(this->renderer.get(), "images/faces.png"));
    if (!this->image) {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }

    this->src_rects = createRects(this->image, FACE_SIZE, FACE_SIZE);

    this->setScale(this->scale);
}

void Face::setScale(int new_scale) {
    this->scale = new_scale;

    this->dest_rect.x =
        ((PIECE_SIZE * static_cast<int>(this->columns) - FACE_SIZE) / 2 +
         PIECE_SIZE - BORDER_LEFT) *
        this->scale;
    this->dest_rect.y = FACE_TOP * this->scale;
    this->dest_rect.w = FACE_SIZE * this->scale;
    this->dest_rect.h = FACE_SIZE * this->scale;
}

void Face::setTheme(std::size_t new_theme) { this->theme = new_theme * 5; }

bool Face::mouseClick(int x, int y, bool down) {
    if (x >= this->dest_rect.x && x <= this->dest_rect.x + this->dest_rect.w) {
        if (y >= this->dest_rect.y &&
            y <= this->dest_rect.y + this->dest_rect.h) {
            if (down) {
                this->image_index = 1;
            } else if (this->image_index == 1) {
                this->image_index = 0;
                return true;
            }
        }
    } else if (!down) {
        this->image_index = 0;
    }

    return false;
}

void Face::draw() const {
    SDL_RenderCopy(this->renderer.get(), this->image.get(),
                   &this->src_rects[this->image_index + this->theme],
                   &this->dest_rect);
}
