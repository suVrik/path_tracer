#pragma once

#include "maths.h"

float fresnel_dielectric(float incident_cos_theta, float incident_ior, float transmitted_ior);
float3 faceforward(const float3& n, const float3& v);
bool refract(const float3& incident, const float3& normal, float ior, float3& transmitted);

class Material {
public:
    virtual ~Material() = default;

    virtual float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const;

    virtual float3 bsdf(const float3& ingoing, const float3& outgoing) const;

    virtual float3 emissive() const;

    virtual bool is_specular() const;
};
