#include "geometry/sphere_geometry.h"

#include <cassert>

SphereGeometry::SphereGeometry(float radius)
    : m_radius(radius)
{
    assert(radius > 0.f);
}

std::optional<GeometryHit> SphereGeometry::raycast(const float3& origin, const float3& direction, float length) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.f));
    assert(length > 0.f);

    float B = 2 * dot(direction, origin);
    float C = dot(origin, origin) - sqr(m_radius);
    float D = sqr(B) - 4 * C;

    if (D >= 0.f) {
        float sqrt_D = std::sqrt(D);

        float distance = (-B - sqrt_D) / 2.f;
        if (distance <= EPSILON) {
            distance = (-B + sqrt_D) / 2.f;
        }

        if (distance > EPSILON && distance <= length) {
            float3 position = origin + direction * distance;
            float3 normal = normalize(position / m_radius);
            float3 tangent;
            if (!equal(std::abs(normal.z), 1.f)) {
                tangent = normalize(cross(normal, float3(0.f, 0.f, 1.f)));
            } else {
                tangent = float3(1.f, 0.f, 0.f);
            }
            float3 bitangent = cross(normal, tangent);
            return GeometryHit{ position, tangent, bitangent, normal, distance };
        }
    }

    return std::nullopt;
}

GeometrySample SphereGeometry::sample(const float2& random) const {
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    float z = 1.f - 2.f * random[0];
    float r = std::sqrt(std::max(0.f, 1.f - sqr(z)));
    float phi = 2.f * PI * random[1];

    GeometrySample result;
    result.position = m_radius * float3(r * std::cos(phi), r * std::sin(phi), z);
    result.normal = normalize(result.position);
    if (!equal(std::abs(result.normal.z), 1.f)) {
        result.tangent = normalize(cross(result.normal, float3(0.f, 0.f, 1.f)));
    } else {
        result.tangent = float3(1.f, 0.f, 0.f);
    }
    result.bitangent = cross(result.normal, result.tangent);
    return result;
}

float SphereGeometry::area() const {
    return 4.f * PI * sqr(m_radius);
}
