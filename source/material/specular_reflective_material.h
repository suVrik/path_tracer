#pragma once

#include "material/material.h"

class SpecularReflectiveMaterial : public Material {
public:
    SpecularReflectiveMaterial(float ior);

    float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const override;

    bool is_specular() const override;

private:
    float m_ior;
};
