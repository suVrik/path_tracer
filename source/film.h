#pragma once

#include "maths.h"

#include <memory>

static constexpr int TILE_SIZE = 64;

class Film {
public:
    Film(int width, int height);

    void blit(void* rgba, int pitch);

    void add_sample(float x, float y, const float3& sample);

    const int width;
    const int height;

    const int tiles_x;
    const int tiles_y;

private:
    struct Pixel {
        float3 samples;
        float divider = 0.f;
    };

    struct Tile {
        Pixel pixels[TILE_SIZE][TILE_SIZE];
    };

    std::unique_ptr<Tile[]> m_tiles;
};
