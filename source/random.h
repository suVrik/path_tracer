#pragma once

#include "maths.h"

#include <random>

class Random {
public:
    Random(int seed);

    double rand();
    float2 rand2();

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<double> m_distribution;
};
