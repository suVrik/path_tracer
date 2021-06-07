#pragma once

#include "geometry/geometry.h"
#include "material/material.h"

#include <memory>

class Primitive {
public:
    Primitive(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material, const float4x4& transform);

    std::optional<GeometryHit> geometry_raycast(const float3& origin, const float3& direction, double length) const;
    GeometrySample geometry_sample(const float2& random) const;
    double geometry_pdf(const float3& origin, const float3& direction) const;

    float3 material_bsdf(float3& ingoing, const float3& outgoing, const float2& random) const;
    float3 material_bsdf(const float3& ingoing, const float3& outgoing) const;
    float3 material_emissive() const;
    bool is_material_specular() const;

private:
    std::shared_ptr<Geometry> m_geometry;
    std::shared_ptr<Material> m_material;
    float4x4 m_transform;
    float4x4 m_inv_transform;
};
