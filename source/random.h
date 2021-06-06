#pragma once

#include "maths.h"

#include <random>
#include <vector>

class Random {
public:
    Random(int seed, int total_samples);

    double rand();
    float2 rand2();

    double rand(int sample);
    float2 rand2(int sample);

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<double> m_distribution;

    int m_sample_count_1d;
    int m_sample_count_2d;

    std::vector<int> m_sample_mapping_1d;
    std::vector<int> m_sample_mapping_2d;
};
