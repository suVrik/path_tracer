#pragma once

#include "maths.h"

class Material {
public:
    virtual ~Material() = default;

    virtual float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const;

    virtual float3 bsdf(const float3& ingoing, const float3& outgoing) const;

    virtual float3 emissive() const;

    virtual bool is_specular() const;
};

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const float3& albedo);

    float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const override;
    float3 bsdf(const float3& ingoing, const float3& outgoing) const override;

private:
    float3 m_albedo;
};

class EmissiveMaterial : public Material {
public:
    EmissiveMaterial(const float3& emissive);

    float3 emissive() const override;

private:
    float3 m_emissive;
};

class SpecularReflectiveMaterial : public Material {
public:
    SpecularReflectiveMaterial(float ior);

    float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const override;

    bool is_specular() const override;

private:
    float m_ior;
};

class SpecularTransmissiveMaterial : public Material {
public:
    SpecularTransmissiveMaterial(float ior);

    float3 bsdf(float3& ingoing, const float3& outgoing, const float2& random) const override;

    bool is_specular() const override;

private:
    float m_ior;
};
