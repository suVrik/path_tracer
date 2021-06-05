#include "material.h"

#include <algorithm>
#include <cassert>

double fresnel_dielectric(double incident_cos_theta, double incident_ior, double transmitted_ior) {
    assert(incident_cos_theta >= -1.0 && incident_cos_theta != 0.0 && incident_cos_theta <= 1.0);
    assert(incident_ior >= 1.0);
    assert(transmitted_ior >= 1.0);

    if (incident_cos_theta < 0.0) {
        std::swap(incident_ior, transmitted_ior);
        incident_cos_theta = -incident_cos_theta;
    }

    double incident_sin_theta = std::sqrt(1.0 - sqr(incident_cos_theta));

    double transmitted_sin_theta = incident_ior / transmitted_ior * incident_sin_theta;
    if (transmitted_sin_theta >= 1.0) {
        return 1.0;
    }

    double transmitted_cos_theta = std::sqrt(1.0 - sqr(transmitted_sin_theta));

    double reflectance_parallel =
        ((transmitted_ior * incident_cos_theta) - (incident_ior * transmitted_cos_theta)) /
        ((transmitted_ior * incident_cos_theta) + (incident_ior * transmitted_cos_theta));
    double reflectance_perpendicular =
        ((incident_ior * incident_cos_theta) - (transmitted_ior * transmitted_cos_theta)) /
        ((incident_ior * incident_cos_theta) + (transmitted_ior * transmitted_cos_theta));
    
    return (sqr(reflectance_parallel) + sqr(reflectance_perpendicular)) / 2.0;
}

float3 faceforward(const float3& n, const float3& v) {
    return (dot(n, v) < 0.0) ? -n : n;
}

bool refract(const float3& incident, const float3& normal, double ior, float3& transmitted) {
    double incident_cos_theta = dot(normal, incident);
    double incident_sin2_theta = std::max(0.0, 1.0 - sqr(incident_cos_theta));
    double transmitted_sin2_theta = incident_sin2_theta * sqr(ior);
    
    if (transmitted_sin2_theta >= 1.0) {
        transmitted = float3(-incident.x, -incident.y, incident.z);
        return false;
    }

    double transmitted_cos_theta = std::sqrt(std::max(0.0, 1.0 - transmitted_sin2_theta));
    transmitted = -incident * ior + normal * (incident_cos_theta * ior - transmitted_cos_theta);

    return true;
}

float3 Material::bsdf(float3& ingoing, const float3& outgoing, const float2& random) const {
    assert(equal(length(outgoing), 1.0));
    assert(random[0] >= 0.0 && random[0] < 1.0);
    assert(random[1] >= 0.0 && random[1] < 1.0);

    ingoing = float3(-outgoing.x, -outgoing.y, outgoing.z);

    return float3();
}

float3 Material::bsdf(const float3& ingoing, const float3& outgoing) const {
    assert(equal(length(ingoing), 1.0));
    assert(equal(length(outgoing), 1.0) && !equal(outgoing.z, 0.0));

    return float3();
}

float3 Material::emissive() const {
    return float3();
}

bool Material::is_specular() const {
    return false;
}
