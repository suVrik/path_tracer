#pragma once

#include "maths.h"

// TODO: Get rid of this.
constexpr int FILTER_RADIUS = 0;

class Filter {
public:
    // Filter(float radius);
    // virtual ~Filter() = default;

    // TODO: virtual float contribution(float x, float y) const = 0;
    float contribution(int x, int y) const;

    // const float radius;

protected:
    // TODO: Get rid of this.
    float m_contribution[FILTER_RADIUS + 1][FILTER_RADIUS + 1];
};

class BoxFilter : public Filter {
public:
    // BoxFilter(float radius);
    BoxFilter();

    // TODO: float contribution(float x, float y) const override;
};

class TriangleFilter : public Filter {
public:
    // TriangleFilter(float radius);
    TriangleFilter();

    // TODO: float contribution(float x, float y) const override;
};

class GaussianFilter : public Filter {
public:
    // GaussianFilter(float radius, float alpha);
    GaussianFilter(float alpha);

    // TODO: float contribution(float x, float y) const override;
};
