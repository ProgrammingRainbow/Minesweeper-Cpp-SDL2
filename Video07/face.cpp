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

void Face::draw() const {
    SDL_RenderCopy(this->renderer.get(), this->image.get(), &this->src_rects[0],
                   &this->dest_rect);
}
