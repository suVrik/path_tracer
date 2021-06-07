#pragma once

#include "geometry/geometry.h"

class SphereGeometry : public Geometry {
public:
    SphereGeometry(double radius);

    std::optional<GeometryHit> raycast(const float3& origin, const float3& direction, double length) const override;

    GeometrySample sample(const float2& random) const override;

    double pdf(const float3& origin, const float3& direction) const override;

private:
    double m_radius;
};
