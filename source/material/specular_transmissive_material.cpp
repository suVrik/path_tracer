#include "material/specular_transmissive_material.h"

#include <cassert>

SpecularTransmissiveMaterial::SpecularTransmissiveMaterial(double ior)
    : m_ior(ior)
{
    assert(m_ior >= 1.0);
}

float3 SpecularTransmissiveMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.0) && !equal(outgoing.z, 0.0));
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    bool entering = outgoing.z > 0.0;
    double incident_ior = entering ? 1.0 : m_ior;
    double transmitted_ior = entering ? m_ior : 1.0;

    if (!refract(outgoing, faceforward(float3(0.0, 0.0, 1.0), outgoing), incident_ior / transmitted_ior, ingoing)) {
        return float3(0.0);
    }

    return float3((1.0 - fresnel_dielectric(ingoing.z, transmitted_ior, incident_ior)) / std::abs(ingoing.z));
}

bool SpecularTransmissiveMaterial::is_specular() const {
    return true;
}
