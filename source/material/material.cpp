#include "material.h"

#include <algorithm>
#include <cassert>

float fresnel_dielectric(float incident_cos_theta, float incident_ior, float transmitted_ior) {
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

float3 faceforward(const float3& n, const float3& v) {
    return (dot(n, v) < 0.f) ? -n : n;
}

bool refract(const float3& incident, const float3& normal, float ior, float3& transmitted) {
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
