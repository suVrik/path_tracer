#pragma once

#include "geometry/geometry.h"

class BoxGeometry : public Geometry {
public:
    BoxGeometry(float3 half_extents);

    std::optional<GeometryHit> raycast(const float3& origin, const float3& direction, double length) const override;

    GeometrySample sample(const float2& random) const override;

    double pdf(const float3& origin, const float3& direction) const override;

private:
    float3 m_half_extents;
};
