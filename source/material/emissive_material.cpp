#include "material/emissive_material.h"

#include <cassert>

EmissiveMaterial::EmissiveMaterial(const float3& emissive)
    : m_emissive(emissive)
{
    assert(emissive.x >= 0.0 && emissive.y >= 0.0 && emissive.z >= 0.0);
}

float3 EmissiveMaterial::emissive() const {
    return m_emissive;
}
