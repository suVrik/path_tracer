#include "film.h"

#include <algorithm>
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
    , m_tiles(std::make_unique<Tile[]>(tiles_y * tiles_x)) {
}

void Film::blit(void* rgba, int pitch) {
    auto* data = static_cast<char*>(rgba);
    assert(data != nullptr);
    
    for (int i = 0; i < tiles_y; i++) {
        for (int j = 0; j < tiles_x; j++) {
            Tile& tile = m_tiles[i * tiles_x + j];

            int x_from = j * TILE_SIZE;
            int y_from = i * TILE_SIZE;

            int tile_width = std::min(x_from + TILE_SIZE, width) - x_from;
            int tile_height = std::min(y_from + TILE_SIZE, height) - y_from;

            std::lock_guard<Spinlock> lock(tile.spinlock);
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
    int screen_x = static_cast<int>(x);
    int screen_y = static_cast<int>(y);

    if (screen_x >= 0 && screen_x < width && screen_y >= 0 && screen_y < height) {
        int tile_x = screen_x / TILE_SIZE;
        int tile_y = screen_y / TILE_SIZE;

        int pixel_x = screen_x - tile_x * TILE_SIZE;
        int pixel_y = screen_y - tile_y * TILE_SIZE;

        Tile& tile = m_tiles[tile_y * tiles_x + tile_x];
        Pixel& pixel = tile.pixels[pixel_y][pixel_x];

        std::lock_guard<Spinlock> lock(tile.spinlock);
        pixel.samples += sample;
        pixel.divider += 1.f;
    }
}

//void Film::add_samples(int tile_x, int tile_y, const float3 samples[TILE_SIZE][TILE_SIZE]) {
//    assert(tile_x >= 0 && tile_x < tiles_x);
//    assert(tile_y >= 0 && tile_y < tiles_y);
//
//    if constexpr (FILTER_RADIUS == 0) {
//        Tile& tile = m_tiles[tile_y * tiles_x + tile_x];
//
//        std::lock_guard<Spinlock> lock(tile.spinlock);
//        for (int i = 0; i < TILE_SIZE; i++) {
//            for (int j = 0; j < TILE_SIZE; j++) {
//                tile.pixels[i][j].samples += samples[i][j];
//                tile.pixels[i][j].divider++;
//            }
//        }
//    } else {
//        int tx_from = std::max(-tile_x, -1);
//        int ty_from = std::max(-tile_y, -1);
//
//        int tx_to = std::min(tiles_x - tile_x, 2);
//        int ty_to = std::min(tiles_y - tile_y, 2);
//
//        for (int ty = ty_from; ty < ty_to; ty++) {
//            for (int tx = tx_from; tx < tx_to; tx++) {
//                assert(tile_x + tx >= 0 && tile_x + tx < tiles_x);
//                assert(tile_y + ty >= 0 && tile_y + ty < tiles_y);
//
//                int dx_from = std::max(0, -tx * (TILE_SIZE - FILTER_RADIUS));
//                int dy_from = std::max(0, -ty * (TILE_SIZE - FILTER_RADIUS));
//
//                int dx_to = TILE_SIZE - std::max(0, tx * (TILE_SIZE - FILTER_RADIUS));
//                int dy_to = TILE_SIZE - std::max(0, ty * (TILE_SIZE - FILTER_RADIUS));
//
//                Tile& tile = m_tiles[(tile_y + ty) * tiles_x + tile_x + tx];
//
//                std::lock_guard<Spinlock> lock(tile.spinlock);
//
//                for (int dy = dy_from; dy < dy_to; dy++) {
//                    for (int dx = dx_from; dx < dx_to; dx++) {
//                        assert(dx >= 0 && dx < TILE_SIZE);
//                        assert(dy >= 0 && dy < TILE_SIZE);
//
//                        int sx_from = std::max(0, tx * TILE_SIZE + dx - FILTER_RADIUS);
//                        int sy_from = std::max(0, ty * TILE_SIZE + dy - FILTER_RADIUS);
//
//                        int sx_to = std::min(TILE_SIZE, tx * TILE_SIZE + dx + FILTER_RADIUS);
//                        int sy_to = std::min(TILE_SIZE, ty * TILE_SIZE + dy + FILTER_RADIUS);
//
//                        for (int sy = sy_from; sy < sy_to; sy++) {
//                            for (int sx = sx_from; sx < sx_to; sx++) {
//                                assert(sx >= 0 && sx < TILE_SIZE);
//                                assert(sy >= 0 && sy < TILE_SIZE);
//
//                                float contribution = m_filter.contribution(tx * TILE_SIZE + dx - sx, ty * TILE_SIZE + dy - sy);
//
//                                tile.pixels[dy][dx].samples += samples[sy][sx] * contribution;
//                                tile.pixels[dy][dx].divider += contribution;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//}
