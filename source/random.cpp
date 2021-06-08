#include "random.h"

Random::Random(int seed)
    : m_generator(seed)
    , m_distribution(0.0, 1.0)
{
}

double Random::rand() {
    return m_distribution(m_generator);
}

float2 Random::rand2() {
    return float2(rand(), rand());
}
