#pragma once

#include "material/material.h"

class SpecularTransmissiveMaterial : public Material {
public:
    SpecularTransmissiveMaterial(double ior);

    float3 bsdf(float3& ingoing, const float3& outgoing, double& pdf, const float2& random) const override;

    bool is_specular() const override;

private:
    double m_ior;
};
