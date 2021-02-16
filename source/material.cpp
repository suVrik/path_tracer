#include "material.h"

#include <cassert>
#include <utility>

static float fresnel_dielectric(float incident_cos_theta, float incident_ior, float transmitted_ior) {
    assert(incident_cos_theta >= -1.f && incident_cos_theta != 0.f && incident_cos_theta <= 1.f);
    assert(incident_ior >= 1.f);
    assert(transmitted_ior >= 1.f);

    if (incident_cos_theta < 0.f) {
        std::swap(incident_ior, transmitted_ior);
        incident_cos_theta = -incident_cos_theta;
    }

    float incident_sin_theta = std::sqrt(1.f - sqr(incident_cos_theta));

    float transmitted_sin_theta = incident_ior / transmitted_ior * incident_sin_theta;
    if (transmitted_sin_theta >= 1.f) {
        return 1.f;
    }

    float transmitted_cos_theta = std::sqrt(1.f - sqr(transmitted_sin_theta));

    float reflectance_parallel =
        ((transmitted_ior * incident_cos_theta) - (incident_ior * transmitted_cos_theta)) /
        ((transmitted_ior * incident_cos_theta) + (incident_ior * transmitted_cos_theta));
    float reflectance_perpendicular =
        ((incident_ior * incident_cos_theta) - (transmitted_ior * transmitted_cos_theta)) /
        ((incident_ior * incident_cos_theta) + (transmitted_ior * transmitted_cos_theta));
    
    return (sqr(reflectance_parallel) + sqr(reflectance_perpendicular)) / 2.f;
}

inline float3 faceforward(const float3& n, const float3& v) {
    return (dot(n, v) < 0.f) ? -n : n;
}

static bool refract(const float3& incident, const float3& normal, float ior, float3& transmitted) {
    float incident_cos_theta = dot(normal, incident);
    float incident_sin2_theta = std::max(0.f, 1.f - sqr(incident_cos_theta));
    float transmitted_sin2_theta = incident_sin2_theta * sqr(ior);
    
    if (transmitted_sin2_theta >= 1.f) {
        transmitted = float3(-incident.x, -incident.y, incident.z);
        return false;
    }

    float transmitted_cos_theta = std::sqrt(std::max(0.f, 1.f - transmitted_sin2_theta));
    transmitted = -incident * ior + normal * (incident_cos_theta * ior - transmitted_cos_theta);

    return true;
}

//////////////////////////////////////////////////////////////////////////

float3 Material::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f));
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    ingoing = float3(-outgoing.x, -outgoing.y, outgoing.z);

    return float3();
}

float3 Material::bsdf(const float3& ingoing, const float3& outgoing) const {
    assert(equal(length(ingoing), 1.f));
    assert(equal(length(outgoing), 1.f) && !equal(outgoing.z, 0.f));

    return float3();
}

float3 Material::emissive() const {
    return float3();
}

bool Material::is_specular() const {
    return false;
}

//////////////////////////////////////////////////////////////////////////

DiffuseMaterial::DiffuseMaterial(const float3& albedo)
    : m_albedo(albedo) {
    assert(albedo.x >= 0.f && albedo.y >= 0.f && albedo.z >= 0.f);
}

float3 DiffuseMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f));
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    if (outgoing.z < EPSILON) {
        return float3();
    }

    ingoing = sample_hemisphere(random);

    float3 brdf = m_albedo / PI;
    float pdf = 1.f / (2 * PI);

    return brdf / pdf;
}

float3 DiffuseMaterial::bsdf(const float3& ingoing, const float3& outgoing) const {
    assert(equal(length(ingoing), 1.f));
    assert(equal(length(outgoing), 1.f));

    if (outgoing.z < EPSILON) {
        return float3();
    }

    return m_albedo / PI;
}

//////////////////////////////////////////////////////////////////////////

EmissiveMaterial::EmissiveMaterial(const float3& emissive)
    : m_emissive(emissive) {
    assert(emissive.x >= 0.f && emissive.y >= 0.f && emissive.z >= 0.f);
}

float3 EmissiveMaterial::emissive() const {
    return m_emissive;
}

//////////////////////////////////////////////////////////////////////////

SpecularReflectiveMaterial::SpecularReflectiveMaterial(float ior)
    : m_ior(ior) {
    assert(m_ior >= 1.f);
}

float3 SpecularReflectiveMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f));
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    if (outgoing.z < EPSILON) {
        return float3();
    }

    ingoing = float3(-outgoing.x, -outgoing.y, outgoing.z);

    return float3(fresnel_dielectric(ingoing.z, 1.f, m_ior) / std::abs(ingoing.z));
}

bool SpecularReflectiveMaterial::is_specular() const {
    return true;
}

//////////////////////////////////////////////////////////////////////////

SpecularTransmissiveMaterial::SpecularTransmissiveMaterial(float ior)
    : m_ior(ior) {
    assert(m_ior >= 1.f);
}

float3 SpecularTransmissiveMaterial::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.f) && !equal(outgoing.z, 0.f));
    assert(random[0] >= 0.f && random[0] < 1.f);
    assert(random[1] >= 0.f && random[1] < 1.f);

    bool entering = outgoing.z > 0.f;
    float incident_ior = entering ? 1.f : m_ior;
    float transmitted_ior = entering ? m_ior : 1.f;

    if (!refract(outgoing, faceforward(float3(0.f, 0.f, 1.f), outgoing), incident_ior / transmitted_ior, ingoing)) {
        return float3(0.f);
    }

    return float3((1.f - fresnel_dielectric(ingoing.z, transmitted_ior, incident_ior)) / std::abs(ingoing.z));
}

bool SpecularTransmissiveMaterial::is_specular() const {
    return true;
}

//////////////////////////////////////////////////////////////////////////
