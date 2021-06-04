#include "material/diffuse_material.h"

#include <cassert>

DiffuseMaterial::DiffuseMaterial(const float3& albedo)
    : m_albedo(albedo)
{
    assert(albedo.x >= 0.f && albedo.y >= 0.f && albedo.z >= 0.f);
}

float3 DiffuseMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f));
    assert(random[0] >= 0.f && random[0] <= 1.f);
    assert(random[1] >= 0.f && random[1] <= 1.f);

    if (outgoing.z < EPSILON) {
        ingoing = float3(0.f, 0.f, -1.f);
        return float3();
    }

    ingoing = sample_hemisphere(random);

    float3 brdf = m_albedo / PI;
    float pdf = 1.f / (2 * PI);

    return brdf / pdf;
}

float3 DiffuseMaterial::bsdf(const float3& ingoing, const float3& outgoing) const {
    assert(equal(length(ingoing), 1.f));
    assert(equal(length(outgoing), 1.f));

    if (outgoing.z < EPSILON) {
        return float3();
    }

    return m_albedo / PI;
}
