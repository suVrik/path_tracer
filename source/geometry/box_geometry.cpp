#include "geometry/box_geometry.h"

#include <cassert>

BoxGeometry::BoxGeometry(float3 half_extents)
    : m_half_extents(half_extents)
{
    assert(half_extents.x > 0.f);
    assert(half_extents.y > 0.f);
    assert(half_extents.z > 0.f);
}

std::optional<GeometryHit> BoxGeometry::raycast(const float3& origin, const float3& direction, float length) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.f));
    assert(length > 0.f);

    float near = 0.f;
    int near_normal_index = 0;
    float near_normal_sign = 1.f;

    float far = length;
    int far_normal_index = 0;
    float far_normal_sign = 1.f;

    for (int i = 0; i < 3; i++) {
        float inverse_direction = 1.f / direction[i];

        float t1 = (m_half_extents[i] - origin[i]) * inverse_direction;
        float t2 = (-m_half_extents[i] - origin[i]) * inverse_direction;

        float normal_sign = 1.f;

        if (t1 > t2) {
            std::swap(t1, t2);

            normal_sign = -1.f;
        }

        if (t1 > near) {
            near = t1;

            near_normal_index = i;
            near_normal_sign = normal_sign;
        }

        if (t2 < far) {
            far = t2;

            far_normal_index = i;
            far_normal_sign = normal_sign;
        }

        if (near > far) {
            return std::nullopt;
        }
    }

    if (std::abs(origin.x) < m_half_extents.x + EPSILON && std::abs(origin.y) < m_half_extents.y + EPSILON && std::abs(origin.z) < m_half_extents.z + EPSILON) {
        near = far;
        near_normal_index = far_normal_index;
        near_normal_sign = -far_normal_sign;
    }

    if (equal(near, 0.f)) {
        return std::nullopt;
    }

    int normal_index = near_normal_index;
    int tangent_index = (normal_index + 1) % 3;
    int bitangent_index = (normal_index + 2) % 3;

    GeometryHit hit;
    hit.position = origin + direction * near;
    hit.tangent[normal_index] = 0.f;
    hit.tangent[tangent_index] = 1.f;
    hit.tangent[bitangent_index] = 0.f;
    hit.bitangent[normal_index] = 0.f;
    hit.bitangent[tangent_index] = 0.f;
    hit.bitangent[bitangent_index] = 1.f;
    hit.normal[normal_index] = near_normal_sign;
    hit.normal[tangent_index] = 0.f;
    hit.normal[bitangent_index] = 0.f;
    hit.distance = near;
    return hit;
}

static const GeometrySample BOX_SAMPLES[6] = {
    { float3(), float3(0.f, 1.f, 0.f), float3(0.f, 0.f, 1.f), float3(1.f, 0.f, 0.f)  },
    { float3(), float3(0.f, 1.f, 0.f), float3(0.f, 0.f, 1.f), float3(-1.f, 0.f, 0.f) },
    { float3(), float3(0.f, 0.f, 1.f), float3(1.f, 0.f, 0.f), float3(0.f, 1.f, 0.f)  },
    { float3(), float3(0.f, 0.f, 1.f), float3(1.f, 0.f, 0.f), float3(0.f, -1.f, 0.f) },
    { float3(), float3(1.f, 0.f, 0.f), float3(0.f, 1.f, 0.f), float3(0.f, 0.f, 1.f)  },
    { float3(), float3(1.f, 0.f, 0.f), float3(0.f, 1.f, 0.f), float3(0.f, 0.f, -1.f) },
};

GeometrySample BoxGeometry::sample(const float2& random) const {
    assert(random[0] >= 0.f && random[0] <= 1.f);
    assert(random[1] >= 0.f && random[1] <= 1.f);

    int side_index = static_cast<int>(random[0] * 6);
    assert(side_index >= 0 && side_index < 6);

    float u = lerp(-1.f, 1.f, clamp(random[0] * 6.f - side_index, 0.f, 1.f));
    float v = lerp(-1.f, 1.f, random[1]);

    GeometrySample result = BOX_SAMPLES[side_index];
    result.position = m_half_extents * (result.normal + u * result.tangent + v * result.bitangent);
    return result;
}

float BoxGeometry::area() const {
    return (m_half_extents.x * m_half_extents.y + m_half_extents.x * m_half_extents.z + m_half_extents.y * m_half_extents.z) * 2.f;
}