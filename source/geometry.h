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

class BoxGeometry : public Geometry {
public:
    BoxGeometry(float3 half_extents);

    std::optional<GeometryHit> raycast(const float3& origin, const float3& direction, float length) const override;

    GeometrySample sample(const float2& random) const override;

    float area() const override;

private:
    float3 m_half_extents;
};

class SphereGeometry : public Geometry {
public:
    SphereGeometry(float radius);

    std::optional<GeometryHit> raycast(const float3& origin, const float3& direction, float length) const override;

    GeometrySample sample(const float2& random) const override;

    float area() const override;

private:
    float m_radius;
};
