#include "primitive.h"

#include <cassert>

Primitive::Primitive(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material, const float4x4& transform)
    : m_geometry(std::move(geometry))
    , m_material(std::move(material))
    , m_transform(transform)
    , m_inv_transform(inverse(transform))
{
    assert(m_geometry != nullptr);
    assert(m_material != nullptr);
}

std::optional<GeometryHit> Primitive::geometry_raycast(const float3& origin, const float3& direction, double length) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.0));
    assert(length >= 0.0);

    float3 object_origin(point_transform(origin, m_inv_transform));
    float3 object_direction(normalize(direction * m_inv_transform));

    assert(isfinite(object_origin));
    assert(equal(::length(object_direction), 1.0));

    std::optional<GeometryHit> object_result = m_geometry->raycast(object_origin, object_direction, length);
    if (object_result) {
        assert(isfinite(object_result->position));
        assert(equal(::length(object_result->tangent), 1.0));
        assert(equal(::length(object_result->bitangent), 1.0));
        assert(equal(::length(object_result->normal), 1.0));
        assert(object_result->distance >= 0 && object_result->distance <= length);

        float3 world_position = point_transform(object_result->position, m_transform);
        float3 world_tangent = normalize(object_result->tangent * m_transform);
        float3 world_bitangent = normalize(object_result->bitangent * m_transform);
        float3 world_normal = normalize(normal_transform(object_result->normal, m_inv_transform));

        assert(isfinite(world_position));
        assert(equal(::length(world_tangent), 1.0));
        assert(equal(::length(world_bitangent), 1.0));
        assert(equal(::length(world_normal), 1.0));
        assert(object_result->distance > 0.0 && object_result->distance <= length);

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
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    GeometrySample object_space_sample = m_geometry->sample(random);

    assert(isfinite(object_space_sample.position));
    assert(equal(length(object_space_sample.tangent), 1.0));
    assert(equal(length(object_space_sample.bitangent), 1.0));
    assert(equal(length(object_space_sample.normal), 1.0));

    GeometrySample world_space_sample;
    world_space_sample.position = point_transform(object_space_sample.position, m_transform);
    world_space_sample.tangent = normalize(object_space_sample.tangent * m_transform);
    world_space_sample.bitangent = normalize(object_space_sample.bitangent * m_transform);
    world_space_sample.normal = normalize(normal_transform(object_space_sample.normal, m_inv_transform));

    assert(isfinite(world_space_sample.position));
    assert(equal(length(world_space_sample.tangent), 1.0));
    assert(equal(length(world_space_sample.bitangent), 1.0));
    assert(equal(length(world_space_sample.normal), 1.0));

    return world_space_sample;
}

double Primitive::geometry_pdf(const float3& origin, const float3& direction) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.0));

    float3 object_origin(point_transform(origin, m_inv_transform));
    float3 object_direction(normalize(direction * m_inv_transform));

    assert(isfinite(object_origin));
    assert(equal(::length(object_direction), 1.0));

    double result = m_geometry->pdf(object_origin, object_direction);

    assert(std::isfinite(result));
    assert(result >= 0.0);

    return result;
}

float3 Primitive::material_bsdf(float3& ingoing, const float3& outgoing, double& pdf, const float2& random) const {
    assert(equal(length(outgoing), 1.0));
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    float3 result = m_material->bsdf(ingoing, outgoing, pdf, random);

    assert(result.x >= 0.0 && result.y >= 0.0 && result.z >= 0.0);
    assert(equal(length(ingoing), 1.0));
    assert(pdf >= 0.0);

    return result;
}

float3 Primitive::material_bsdf(const float3& ingoing, const float3& outgoing, double& pdf) const {
    assert(equal(length(ingoing), 1.0));
    assert(equal(length(outgoing), 1.0));

    float3 result = m_material->bsdf(ingoing, outgoing, pdf);

    assert(isfinite(result));
    assert(result.x >= 0.0 && result.y >= 0.0 && result.z >= 0.0);
    assert(pdf >= 0.0);

    return result;
}

float3 Primitive::material_emissive() const {
    float3 result = m_material->emissive();

    assert(isfinite(result));
    assert(result.r >= 0.0 && result.g >= 0.0 && result.b >= 0.0);

    return result;
}

bool Primitive::is_material_specular() const {
    return m_material->is_specular();
}
