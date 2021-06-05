#pragma once

#include "maths.h"

class Random {
public:
    Random(unsigned int seed);

    double rand();
    float2 rand2();
    float3 rand3();

    double rand(int sample, int total_samples);
    float2 rand2(int sample, int total_samples);
    float3 rand3(int sample, int total_samples);

private:
    static constexpr int N = 624;
    static constexpr int M = 397;

    static constexpr unsigned int MATRIX_A   = 0x9908B0DFU;
    static constexpr unsigned int UPPER_MASK = 0x80000000U;
    static constexpr unsigned int LOWER_MASK = 0x7FFFFFFFU;

    unsigned int m_vector[N];
    unsigned int m_index;
};
