#include "film.h"

#include <cassert>
#include <mutex>

static float gamme_correct(float value) {
    if (value <= 0.0031308f) {
        return 12.92f * value;
    }

    return 1.055f * std::pow(value, 1.f / 2.4f) - 0.055f;
}

static float convert(float value) {
    return static_cast<char>(clamp(gamme_correct(value), 0.f, 1.f) * 255.f + 0.5f);
}

Film::Film(int width, int height)
    : width(width)
    , height(height)
    , tiles_x((width + TILE_SIZE - 1) / TILE_SIZE)
    , tiles_y((height + TILE_SIZE - 1) / TILE_SIZE)
    , m_tiles(std::make_unique<Tile[]>(static_cast<size_t>(tiles_y) * tiles_x))
{
    assert(width > 0);
    assert(height > 0);
}

void Film::blit(void* rgba, int pitch) {
    auto* data = static_cast<char*>(rgba);
    assert(data != nullptr);
    
    for (int i = 0; i < tiles_y; i++) {
        for (int j = 0; j < tiles_x; j++) {
            Tile& tile = m_tiles[static_cast<size_t>(i) * tiles_x + j];

            int x_from = j * TILE_SIZE;
            int y_from = i * TILE_SIZE;

            int tile_width = std::min(x_from + TILE_SIZE, width) - x_from;
            int tile_height = std::min(y_from + TILE_SIZE, height) - y_from;

            for (int y = 0; y < tile_height; y++) {
                for (int x = 0; x < tile_width; x++) {
                    float3 spectrum;
                    if (tile.pixels[y][x].divider != 0.f) {
                        spectrum = tile.pixels[y][x].samples / tile.pixels[y][x].divider;
                    }

                    int offset = (i * TILE_SIZE + y) * pitch + (j * TILE_SIZE + x) * 4;

                    data[offset + 0] = 0xFF;
                    data[offset + 1] = convert(spectrum.z);
                    data[offset + 2] = convert(spectrum.y);
                    data[offset + 3] = convert(spectrum.x);
                }
            }
        }
    }
}

void Film::add_sample(float x, float y, const float3& sample) {
    assert(isfinite(sample));
    assert(sample.x >= 0.f && sample.y >= 0.f && sample.z >= 0.f);

    int screen_x = static_cast<int>(x);
    int screen_y = static_cast<int>(y);

    if (screen_x >= 0 && screen_x < width && screen_y >= 0 && screen_y < height) {
        int tile_x = screen_x / TILE_SIZE;
        int tile_y = screen_y / TILE_SIZE;

        int pixel_x = screen_x - tile_x * TILE_SIZE;
        int pixel_y = screen_y - tile_y * TILE_SIZE;

        Tile& tile = m_tiles[static_cast<size_t>(tile_y) * tiles_x + tile_x];
        Pixel& pixel = tile.pixels[pixel_y][pixel_x];

        pixel.samples += sample;
        pixel.divider += 1.f;
    }
}
