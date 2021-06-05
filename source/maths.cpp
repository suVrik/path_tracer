#include "maths.h"

#include <algorithm>
#include <cassert>

float4x4 float4x4::rotation(const float3& axis, double angle) {
    assert(isfinite(axis));
    assert(std::isfinite(angle));
    assert(equal(length(axis), 1.0));

    double cos = std::cos(angle);
    double sin = std::sin(angle);
    float3 tmp = axis * (1.0 - cos);

    return float4x4(cos + axis.x * tmp.x,          axis.y * tmp.x + axis.z * sin, axis.z * tmp.x - axis.y * sin, 0.0,
                    axis.x * tmp.y - axis.z * sin, cos + axis.y * tmp.y,          axis.z * tmp.y + axis.x * sin, 0.0,
                    axis.x * tmp.z + axis.y * sin, axis.y * tmp.z - axis.x * sin, cos + axis.z * tmp.z,          0.0,
                    0.0,                           0.0,                           0.0,                           1.0);
}

float4x4 float4x4::scale(const float3& scale) {
    assert(isfinite(scale));

    return float4x4(scale.x, 0.0,     0.0,     0.0,
                    0.0,     scale.y, 0.0,     0.0,
                    0.0,     0.0,     scale.z, 0.0,
                    0.0,     0.0,     0.0,     1.0);
}

float4x4 float4x4::translation(const float3& translation) {
    assert(isfinite(translation));

    return float4x4(1.0,           0.0,           0.0,           0.0,
                    0.0,           1.0,           0.0,           0.0,
                    0.0,           0.0,           1.0,           0.0,
                    translation.x, translation.y, translation.z, 1.0);
}

float4x4 float4x4::perspective(double fov_y, double aspect, double z_near, double z_far) {
    assert(std::isfinite(fov_y));
    assert(std::isfinite(aspect));
    assert(std::isfinite(z_near));
    assert(std::isfinite(z_far));
    assert(!equal(aspect, 0.0));
    assert(!equal(fov_y, 0.0));
    assert(!equal(z_near, z_far));

    double multiplier = 1.0 / std::tan(fov_y * 0.5);
    return float4x4(multiplier / aspect, 0.0,        0.0,                               0.0,
                    0.0,                 multiplier, 0.0,                               0.0,
                    0.0,                 0.0,        z_far / (z_far - z_near),          1.0,
                    0.0,                 0.0,        z_far * z_near / (z_near - z_far), 0.0);
}

float3 sample_hemisphere(const float2& random) {
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    double z = random[0];
    double r = std::sqrt(std::max(0.0, 1.0 - sqr(z)));
    double phi = 2 * PI * random[1];
    return float3(r * std::cos(phi), r * std::sin(phi), z);
}
