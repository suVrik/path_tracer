#pragma once

#include "material/material.h"

class EmissiveMaterial : public Material {
public:
    EmissiveMaterial(const float3& emissive);

    float3 emissive() const override;

private:
    float3 m_emissive;
};
