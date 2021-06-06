#include "random.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <numeric>
#include <random>

Random::Random(int seed, int total_samples)
    : m_generator(seed)
    , m_distribution(0.0, 1.0)
    , m_sample_count_1d(total_samples)
    , m_sample_count_2d(static_cast<int>(std::sqrt(total_samples)))
    , m_sample_mapping_1d(m_sample_count_1d)
    , m_sample_mapping_2d(m_sample_count_2d)
{
    std::iota(m_sample_mapping_1d.begin(), m_sample_mapping_1d.end(), 0);
    std::iota(m_sample_mapping_2d.begin(), m_sample_mapping_2d.end(), 0);

    std::shuffle(m_sample_mapping_1d.begin(), m_sample_mapping_1d.end(), std::default_random_engine(static_cast<unsigned int>(seed)));
    std::shuffle(m_sample_mapping_2d.begin(), m_sample_mapping_2d.end(), std::default_random_engine(static_cast<unsigned int>(seed)));
}

double Random::rand() {
    return m_distribution(m_generator);
}

float2 Random::rand2() {
    return float2(rand(), rand());
}

double Random::rand(int sample) {
    if (sample < m_sample_count_1d) {
        return (m_sample_mapping_1d[sample] + rand()) / m_sample_count_1d;
    }
    return rand();
}

float2 Random::rand2(int sample) {
    if (sample < m_sample_count_2d * m_sample_count_2d) {
        return float2((m_sample_mapping_2d[sample % m_sample_count_2d] + rand()) / m_sample_count_2d,
                      (m_sample_mapping_2d[sample / m_sample_count_2d] + rand()) / m_sample_count_2d);
    }
    return rand2();
}
