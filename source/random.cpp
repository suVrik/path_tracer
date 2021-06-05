#include "random.h"

#include <algorithm>

Random::Random(unsigned int seed) {
    m_vector[0] = seed;
    for (m_index = 1; m_index < N; m_index++) {
        m_vector[m_index] = (1812433253U * (m_vector[m_index - 1] ^ (m_vector[m_index - 1] >> 30)) + m_index);
    }
}

double Random::rand() {
    unsigned int result;

    unsigned int mag01[2] = { 0x0U, MATRIX_A };

    if (m_index >= N) {
        int i;

        for (i = 0; i < N - M; i++) {
            result = (m_vector[i] & UPPER_MASK) | (m_vector[i + 1] & LOWER_MASK);
            m_vector[i] = m_vector[i + M] ^ (result >> 1) ^ mag01[result & 0x1U];
        }

        for (; i < N - 1; i++) {
            result = (m_vector[i] & UPPER_MASK) | (m_vector[i + 1] & LOWER_MASK);
            m_vector[i] = m_vector[i + (M - N)] ^ (result >> 1) ^ mag01[result & 0x1U];
        }
        
        result = (m_vector[N - 1] & UPPER_MASK) | (m_vector[0] & LOWER_MASK);
        m_vector[N - 1] = m_vector[M - 1] ^ (result >> 1) ^ mag01[result & 0x1U];

        m_index = 0;
    }

    result = m_vector[m_index++];

    result ^= (result >> 11);
    result ^= (result << 7) & 0x9D2C5680U;
    result ^= (result << 15) & 0xEFC60000U;
    result ^= (result >> 18);

    return result / 4294967296.0;
}

float2 Random::rand2() {
    return float2(rand(), rand());
}

float3 Random::rand3() {
    return float3(rand(), rand(), rand());
}

double Random::rand(int sample, int total_samples) {
    return (sample + rand()) / total_samples;
}

float2 Random::rand2(int sample, int total_samples) {
    int axis = static_cast<int>(std::sqrt(total_samples));
    if (sample < axis * axis) {
        return float2(rand(sample % axis, axis), rand(sample / axis, axis));
    }
    return rand2();
}

float3 Random::rand3(int sample, int total_samples) {
    int axis = static_cast<int>(std::pow(total_samples, 1.0 / 3.0));
    if (sample < axis * axis * axis) {
        return float3(rand(sample % axis, axis), rand(sample / axis % axis, axis), rand(sample / axis / axis, axis));
    }
    return rand3();
}
