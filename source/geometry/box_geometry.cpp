#include "geometry/box_geometry.h"

#include <cassert>

BoxGeometry::BoxGeometry(float3 half_extents)
    : m_half_extents(half_extents)
{
    assert(half_extents.x > 0.0);
    assert(half_extents.y > 0.0);
    assert(half_extents.z > 0.0);
}

std::optional<GeometryHit> BoxGeometry::raycast(const float3& origin, const float3& direction, double length) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.0));
    assert(length > 0.0);

    double near = 0.0;
    int near_normal_index = 0;
    double near_normal_sign = 1.0;

    double far = length;
    int far_normal_index = 0;
    double far_normal_sign = 1.0;

    for (int i = 0; i < 3; i++) {
        double inverse_direction = 1.0 / direction[i];

        double t1 = (m_half_extents[i] - origin[i]) * inverse_direction;
        double t2 = (-m_half_extents[i] - origin[i]) * inverse_direction;

        double normal_sign = 1.0;

        if (t1 > t2) {
            std::swap(t1, t2);

            normal_sign = -1.0;
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

    if (equal(near, 0.0)) {
        return std::nullopt;
    }

    int normal_index = near_normal_index;
    int tangent_index = (normal_index + 1) % 3;
    int bitangent_index = (normal_index + 2) % 3;

    GeometryHit hit;
    hit.position = origin + direction * near;
    hit.tangent[normal_index] = 0.0;
    hit.tangent[tangent_index] = 1.0;
    hit.tangent[bitangent_index] = 0.0;
    hit.bitangent[normal_index] = 0.0;
    hit.bitangent[tangent_index] = 0.0;
    hit.bitangent[bitangent_index] = 1.0;
    hit.normal[normal_index] = near_normal_sign;
    hit.normal[tangent_index] = 0.0;
    hit.normal[bitangent_index] = 0.0;
    hit.distance = near;
    return hit;
}

static const GeometrySample BOX_SAMPLES[6] = {
    { float3(), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0), float3(1.0, 0.0, 0.0)  },
    { float3(), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0), float3(-1.0, 0.0, 0.0) },
    { float3(), float3(0.0, 0.0, 1.0), float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0)  },
    { float3(), float3(0.0, 0.0, 1.0), float3(1.0, 0.0, 0.0), float3(0.0, -1.0, 0.0) },
    { float3(), float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0)  },
    { float3(), float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, -1.0) },
};

GeometrySample BoxGeometry::sample(const float2& random) const {
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    int side_index = static_cast<int>(random[0] * 6);
    assert(side_index >= 0 && side_index < 6);

    double u = lerp(-1.0, 1.0, clamp(random[0] * 6.0 - side_index, 0.0, 1.0));
    double v = lerp(-1.0, 1.0, random[1]);

    GeometrySample result = BOX_SAMPLES[side_index];
    result.position = m_half_extents * (result.normal + u * result.tangent + v * result.bitangent);
    return result;
}

double BoxGeometry::area() const {
    return (m_half_extents.x * m_half_extents.y + m_half_extents.x * m_half_extents.z + m_half_extents.y * m_half_extents.z) * 2.0;
}
