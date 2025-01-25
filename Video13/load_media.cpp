#include "load_media.h"

std::vector<SDL_Rect> createRects(
    const std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> &texture,
    int width, int height) {

    int tex_w = 0;
    int tex_h = 0;

    if (SDL_QueryTexture(texture.get(), nullptr, nullptr, &tex_w, &tex_h) !=
        0) {
        auto error = std::format("Error querying texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    int max_rows = tex_h / height;
    int max_columns = tex_w / width;

    std::vector<SDL_Rect> rects;
    rects.reserve(static_cast<std::size_t>(max_rows * max_columns));

    for (int row = 0; row < max_rows; row++) {
        for (int column = 0; column < max_columns; column++) {
            rects.emplace_back(
                SDL_Rect{column * width, row * height, width, height});
        }
    }

    return rects;
}
