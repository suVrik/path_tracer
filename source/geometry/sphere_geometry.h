#pragma once

#include "geometry/geometry.h"

class SphereGeometry : public Geometry {
public:
    SphereGeometry(float radius);

    std::optional<GeometryHit> raycast(const float3& origin, const float3& direction, float length) const override;

    GeometrySample sample(const float2& random) const override;

    float area() const override;

private:
    float m_radius;
};
