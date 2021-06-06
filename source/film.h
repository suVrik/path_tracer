#pragma once

#include "maths.h"

#include <memory>

static constexpr int TILE_SIZE = 32;

class Film {
public:
    Film(int width, int height);

    void blit(void* rgba, int pitch);

    void add_samples(int tile_x, int tile_y, float3 samples[TILE_SIZE][TILE_SIZE]);

    const int width;
    const int height;

    const int tiles_x;
    const int tiles_y;

private:
    struct Tile {
        float3 samples[TILE_SIZE][TILE_SIZE];
        double divider = 0.0;
    };

    std::unique_ptr<Tile[]> m_tiles;
};
