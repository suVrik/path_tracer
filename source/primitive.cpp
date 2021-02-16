#include "primitive.h"
#include "geometry.h"
#include "material.h"

#include <cassert>

Primitive::Primitive(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material, const float4x4& transform)
    : m_geometry(std::move(geometry))
    , m_material(std::move(material))
    , m_transform(transform)
    , m_inv_transform(inverse(transform)) {
    assert(m_geometry != nullptr);
    assert(m_material != nullptr);
}

std::optional<GeometryHit> Primitive::geometry_raycast(const float3& origin, const float3& direction, float length) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.f));
    assert(length >= 0.f);

    float3 object_origin(point_transform(origin, m_inv_transform));
    float3 object_direction(normalize(direction * m_inv_transform));

    assert(isfinite(object_origin));
    assert(equal(::length(object_direction), 1.f));

    std::optional<GeometryHit> object_result = m_geometry->raycast(object_origin, object_direction, length);
    if (object_result) {
        assert(isfinite(object_result->position));
        assert(equal(::length(object_result->tangent), 1.f));
        assert(equal(::length(object_result->bitangent), 1.f));
        assert(equal(::length(object_result->normal), 1.f));
        assert(object_result->distance >= 0 && object_result->distance <= length);

        float3 world_position = point_transform(object_result->position, m_transform);
        float3 world_tangent = normalize(object_result->tangent * m_transform);
        float3 world_bitangent = normalize(object_result->bitangent * m_transform);
        float3 world_normal = normalize(normal_transform(object_result->normal, m_inv_transform));

        assert(isfinite(world_position));
        assert(equal(::length(world_tangent), 1.f));
        assert(equal(::length(world_bitangent), 1.f));
        assert(equal(::length(world_normal), 1.f));
        assert(object_result->distance > 0.f && object_result->distance <= length);

        GeometryHit world_result;
        world_result.position = world_position;
        world_result.tangent = world_tangent;
        world_result.bitangent = world_bitangent;
        world_result.normal = world_normal;
        world_result.distance = object_result->distance;
        return world_result;
    }

    return std::nullopt;
}

GeometrySample Primitive::geometry_sample(const float2& random) const {
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    GeometrySample object_space_sample = m_geometry->sample(random);

    assert(isfinite(object_space_sample.position));
    assert(equal(length(object_space_sample.tangent), 1.f));
    assert(equal(length(object_space_sample.bitangent), 1.f));
    assert(equal(length(object_space_sample.normal), 1.f));

    GeometrySample world_space_sample;
    world_space_sample.position = point_transform(object_space_sample.position, m_transform);
    world_space_sample.tangent = normalize(object_space_sample.tangent * m_transform);
    world_space_sample.bitangent = normalize(object_space_sample.bitangent * m_transform);
    world_space_sample.normal = normalize(normal_transform(object_space_sample.normal, m_inv_transform));

    assert(isfinite(world_space_sample.position));
    assert(equal(length(world_space_sample.tangent), 1.f));
    assert(equal(length(world_space_sample.bitangent), 1.f));
    assert(equal(length(world_space_sample.normal), 1.f));

    return world_space_sample;
}

float Primitive::geometry_area() const {
    float result = m_geometry->area();

    assert(result >= 0.f);

    return result;
}

float3 Primitive::material_bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f));
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    float3 result = m_material->bsdf(ingoing, outgoing, random);

    assert(result.x >= 0.f && result.y >= 0.f && result.z >= 0.f);
    assert(equal(length(ingoing), 1.f));

    return result;
}

float3 Primitive::material_bsdf(const float3& ingoing, const float3& outgoing) const {
    assert(equal(length(ingoing), 1.f));
    assert(equal(length(outgoing), 1.f));

    float3 result = m_material->bsdf(ingoing, outgoing);

    assert(result.x >= 0.f && result.y >= 0.f && result.z >= 0.f);

    return result;
}

float3 Primitive::material_emissive() const {
    float3 result = m_material->emissive();

    assert(result.r >= 0.f && result.g >= 0.f && result.b >= 0.f);

    return result;
}

bool Primitive::is_material_specular() const {
    return m_material->is_specular();
}
