#include "material/specular_reflective_material.h"

#include <cassert>

SpecularReflectiveMaterial::SpecularReflectiveMaterial(double ior)
    : m_ior(ior)
{
    assert(m_ior >= 1.0);
}

float3 SpecularReflectiveMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.0));
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    if (outgoing.z < EPSILON) {
        return float3();
    }

    ingoing = float3(-outgoing.x, -outgoing.y, outgoing.z);

    return float3(fresnel_dielectric(ingoing.z, 1.0, m_ior) / std::abs(ingoing.z));
}

bool SpecularReflectiveMaterial::is_specular() const {
    return true;
}
