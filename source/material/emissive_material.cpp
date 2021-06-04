#include "material/emissive_material.h"

#include <cassert>

EmissiveMaterial::EmissiveMaterial(const float3& emissive)
    : m_emissive(emissive)
{
    assert(emissive.x >= 0.f && emissive.y >= 0.f && emissive.z >= 0.f);
}

float3 EmissiveMaterial::emissive() const {
    return m_emissive;
}
