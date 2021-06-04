#include "material/specular_reflective_material.h"

#include <cassert>

SpecularReflectiveMaterial::SpecularReflectiveMaterial(float ior)
    : m_ior(ior)
{
    assert(m_ior >= 1.f);
}

float3 SpecularReflectiveMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f));
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    if (outgoing.z < EPSILON) {
        return float3();
    }

    ingoing = float3(-outgoing.x, -outgoing.y, outgoing.z);

    return float3(fresnel_dielectric(ingoing.z, 1.f, m_ior) / std::abs(ingoing.z));
}

bool SpecularReflectiveMaterial::is_specular() const {
    return true;
}
