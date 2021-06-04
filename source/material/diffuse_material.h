#pragma once

#include "material/material.h"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const float3& albedo);

    float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const override;
    float3 bsdf(const float3& ingoing, const float3& outgoing) const override;

private:
    float3 m_albedo;
};
