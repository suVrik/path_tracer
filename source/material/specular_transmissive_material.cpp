#include "material/specular_transmissive_material.h"

#include <cassert>

SpecularTransmissiveMaterial::SpecularTransmissiveMaterial(float ior)
    : m_ior(ior)
{
    assert(m_ior >= 1.f);
}

float3 SpecularTransmissiveMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f) && !equal(outgoing.z, 0.f));
    assert(random[0] >= 0.f && random[0] <= 1.f);
    assert(random[1] >= 0.f && random[1] <= 1.f);

    bool entering = outgoing.z > 0.f;
    float incident_ior = entering ? 1.f : m_ior;
    float transmitted_ior = entering ? m_ior : 1.f;

    if (!refract(outgoing, faceforward(float3(0.f, 0.f, 1.f), outgoing), incident_ior / transmitted_ior, ingoing)) {
        return float3(0.f);
    }

    return float3((1.f - fresnel_dielectric(ingoing.z, transmitted_ior, incident_ior)) / std::abs(ingoing.z));
}

bool SpecularTransmissiveMaterial::is_specular() const {
    return true;
}
