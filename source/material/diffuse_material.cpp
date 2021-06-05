#include "material/diffuse_material.h"

#include <cassert>

DiffuseMaterial::DiffuseMaterial(const float3& albedo)
    : m_albedo(albedo)
{
    assert(albedo.x >= 0.0 && albedo.y >= 0.0 && albedo.z >= 0.0);
}

float3 DiffuseMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.0));
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    if (outgoing.z < EPSILON) {
        ingoing = float3(0.0, 0.0, -1.0);
        return float3();
    }

    ingoing = sample_hemisphere(random);

    float3 brdf = m_albedo / PI;
    double pdf = 1.0 / (2 * PI);

    return brdf / pdf;
}

float3 DiffuseMaterial::bsdf(const float3& ingoing, const float3& outgoing) const {
    assert(equal(length(ingoing), 1.0));
    assert(equal(length(outgoing), 1.0));

    if (outgoing.z < EPSILON) {
        return float3();
    }

    return m_albedo / PI;
}
