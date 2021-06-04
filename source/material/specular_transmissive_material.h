#pragma once

#include "material/material.h"

class SpecularTransmissiveMaterial : public Material {
public:
    SpecularTransmissiveMaterial(float ior);

    float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const override;

    bool is_specular() const override;

private:
    float m_ior;
};