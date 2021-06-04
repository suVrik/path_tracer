#pragma once

#include "maths.h"

#include <optional>

struct GeometryHit {
    float3 position;
    float3 tangent;
    float3 bitangent;
    float3 normal;
    float distance;
};

struct GeometrySample {
    float3 position;
    float3 tangent;
    float3 bitangent;
    float3 normal;
};

class Geometry {
public:
    virtual ~Geometry() = default;

    virtual std::optional<GeometryHit> raycast(const float3& origin, const float3& direction, float length) const = 0;

    virtual GeometrySample sample(const float2& random) const = 0;

    virtual float area() const = 0;
};
