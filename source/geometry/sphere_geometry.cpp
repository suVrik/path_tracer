#include "geometry/sphere_geometry.h"

#include <cassert>

SphereGeometry::SphereGeometry(double radius)
    : m_radius(radius)
{
    assert(radius > 0.0);
}

std::optional<GeometryHit> SphereGeometry::raycast(const float3& origin, const float3& direction, double length) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.0));
    assert(length > 0.0);

    double B = 2 * dot(direction, origin);
    double C = dot(origin, origin) - sqr(m_radius);
    double D = sqr(B) - 4 * C;

    if (D >= 0.0) {
        double sqrt_D = std::sqrt(D);

        double distance = (-B - sqrt_D) / 2.0;
        if (distance <= EPSILON) {
            distance = (-B + sqrt_D) / 2.0;
        }

        if (distance > EPSILON && distance <= length) {
            float3 position = origin + direction * distance;
            float3 normal = normalize(position / m_radius);
            float3 tangent;
            if (!equal(std::abs(normal.z), 1.0)) {
                tangent = normalize(cross(normal, float3(0.0, 0.0, 1.0)));
            } else {
                tangent = float3(1.0, 0.0, 0.0);
            }
            float3 bitangent = cross(normal, tangent);
            return GeometryHit{ position, tangent, bitangent, normal, distance };
        }
    }

    return std::nullopt;
}

GeometrySample SphereGeometry::sample(const float2& random) const {
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    double z = 1.0 - 2.0 * random[0];
    double r = std::sqrt(std::max(0.0, 1.0 - sqr(z)));
    double phi = 2.0 * PI * random[1];

    GeometrySample result;
    result.position = m_radius * float3(r * std::cos(phi), r * std::sin(phi), z);
    result.normal = normalize(result.position);
    if (!equal(std::abs(result.normal.z), 1.0)) {
        result.tangent = normalize(cross(result.normal, float3(0.0, 0.0, 1.0)));
    } else {
        result.tangent = float3(1.0, 0.0, 0.0);
    }
    result.bitangent = cross(result.normal, result.tangent);
    return result;
}

double SphereGeometry::pdf(const float3& origin, const float3& direction) const {
    assert(isfinite(origin));
    assert(equal(::length(direction), 1.0));

    std::optional<GeometryHit> hit = raycast(origin, direction, std::numeric_limits<double>::infinity());
    if (hit) {
        double distance_squared = square_distance(hit->position, origin);
        double consine = std::abs(dot(hit->normal, direction));
        double area = 4.0 * PI * sqr(m_radius);
        return distance_squared / (consine * area);
    }

    return 0.0;
}
