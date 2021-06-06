#include "film.h"

#include <cassert>
#include <mutex>

static double gamma_correct(double value) {
    if (value <= 0.0031308) {
        return 12.92 * value;
    }

    return 1.055 * std::pow(value, 1.0 / 2.4) - 0.055;
}

static double convert(double value) {
    return static_cast<char>(clamp(gamma_correct(value), 0.0, 1.0) * 255.0 + 0.5);
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
                    if (tile.divider != 0.0) {
                        spectrum = tile.samples[y][x] / tile.divider;
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

void Film::add_samples(int tile_x, int tile_y, float3 samples[TILE_SIZE][TILE_SIZE]) {
    assert(tile_x >= 0 && tile_x < tiles_x && tile_y >= 0 && tile_y < tiles_y);

    Tile& tile = m_tiles[static_cast<size_t>(tile_y) * tiles_x + tile_x];
    tile.divider += 1.0;

    for (int y = 0; y < TILE_SIZE; y++) {
        for (int x = 0; x < TILE_SIZE; x++) {
            assert(isfinite(samples[y][x]));
            assert(samples[y][x].r >= 0.0 && samples[y][x].g >= 0.0 && samples[y][x].b >= 0.0);

            tile.samples[y][x] += samples[y][x];
        }
    }
}
