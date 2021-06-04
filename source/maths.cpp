#include "maths.h"

#include <algorithm>
#include <cassert>

float4x4 float4x4::rotation(const float3& axis, float angle) {
    assert(isfinite(axis));
    assert(std::isfinite(angle));
    assert(equal(length(axis), 1.f));

    float cos = std::cos(angle);
    float sin = std::sin(angle);
    float3 tmp = axis * (1.f - cos);

    return float4x4(cos + axis.x * tmp.x,          axis.y * tmp.x + axis.z * sin, axis.z * tmp.x - axis.y * sin, 0.f,
                    axis.x * tmp.y - axis.z * sin, cos + axis.y * tmp.y,          axis.z * tmp.y + axis.x * sin, 0.f,
                    axis.x * tmp.z + axis.y * sin, axis.y * tmp.z - axis.x * sin, cos + axis.z * tmp.z,          0.f,
                    0.f,                           0.f,                           0.f,                           1.f);
}

float4x4 float4x4::scale(const float3& scale) {
    assert(isfinite(scale));

    return float4x4(scale.x, 0.f,     0.f,     0.f,
                    0.f,     scale.y, 0.f,     0.f,
                    0.f,     0.f,     scale.z, 0.f,
                    0.f,     0.f,     0.f,     1.f);
}

float4x4 float4x4::translation(const float3& translation) {
    assert(isfinite(translation));

    return float4x4(1.f,           0.f,           0.f,           0.f,
                    0.f,           1.f,           0.f,           0.f,
                    0.f,           0.f,           1.f,           0.f,
                    translation.x, translation.y, translation.z, 1.f);
}

float4x4 float4x4::perspective(float fov_y, float aspect, float z_near, float z_far) {
    assert(std::isfinite(fov_y));
    assert(std::isfinite(aspect));
    assert(std::isfinite(z_near));
    assert(std::isfinite(z_far));
    assert(!equal(aspect, 0.f));
    assert(!equal(fov_y, 0.f));
    assert(!equal(z_near, z_far));

    float multiplier = 1.f / std::tan(fov_y * 0.5f);
    return float4x4(multiplier / aspect, 0.f,        0.f,                               0.f,
                    0.f,                 multiplier, 0.f,                               0.f,
                    0.f,                 0.f,        z_far / (z_far - z_near),          1.f,
                    0.f,                 0.f,        z_far * z_near / (z_near - z_far), 0.f);
}

float3 sample_hemisphere(const float2& random) {
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    float z = random[0];
    float r = std::sqrt(std::max(0.f, 1.f - sqr(z)));
    float phi = 2 * PI * random[1];
    return float3(r * std::cos(phi), r * std::sin(phi), z);
}
