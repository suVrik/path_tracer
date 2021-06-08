#pragma once

#include "film.h"
#include "integrator/integrator.h"
#include "primitive.h"
#include "random.h"

#include <atomic>
#include <thread>
#include <vector>

class PathTracerIntegrator : public Integrator {
public:
    PathTracerIntegrator(int width, int height, int samples_per_pixel, int max_diffuse_bounces, int max_specular_bounces, std::vector<Primitive>&& primitives);
    ~PathTracerIntegrator() override;

    void blit(void* rgba, int pitch) override;

private:
    void integrate(int thread_index);

    struct PrimitiveHit : GeometryHit {
        const Primitive* primitive;
    };

    float3 sample_ray(Random& random, const float3& origin, const float3& outgoing, int diffuse_bounces, int specular_bounces);
    std::optional<PrimitiveHit> raycast(const float3& origin, const float3& direction) const;

    Film m_film;
    int m_samples_per_pixel;
    int m_max_diffuse_bounces;
    int m_max_specular_bounces;
    std::vector<Primitive> m_primitives;
    std::vector<Primitive*> m_light_primitives;

    int m_thread_count;
    std::vector<std::thread> m_threads;
};
