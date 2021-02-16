#include "filter.h"

#include <algorithm>
#include <cassert>

float Filter::contribution(int x, int y) const {
    assert(std::abs(x) <= FILTER_RADIUS);
    assert(std::abs(y) <= FILTER_RADIUS);

    return m_contribution[std::abs(x)][std::abs(y)];
}

BoxFilter::BoxFilter() {
    for (int i = 0; i <= FILTER_RADIUS; i++) {
        for (int j = 0; j <= FILTER_RADIUS; j++) {
            m_contribution[i][j] = 1.f;
        }
    }
}

TriangleFilter::TriangleFilter() {
    for (int i = 0; i <= FILTER_RADIUS; i++) {
        for (int j = 0; j <= FILTER_RADIUS; j++) {
            m_contribution[i][j] = (FILTER_RADIUS - i) * (FILTER_RADIUS - j);
        }
    }
}

GaussianFilter::GaussianFilter(float alpha) {
    for (int i = 0; i <= FILTER_RADIUS; i++) {
        for (int j = 0; j <= FILTER_RADIUS; j++) {
            //m_contribution[i][j] = gaussian(i, FILTER_RADIUS, alpha) * gaussian(j, FILTER_RADIUS, alpha);
        }
    }
}
