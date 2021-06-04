#pragma once

#include "integrator/integrator.h"
#include "film.h"
#include "primitive.h"

#include <atomic>
#include <thread>
#include <vector>

class BidirectionalPathTracerIntegrator : public Integrator {
public:
    BidirectionalPathTracerIntegrator(int width, int height, int samples_per_pixel, int max_diffuse_bounces, int max_specular_bounces, std::vector<Primitive>&& primitives);
    ~BidirectionalPathTracerIntegrator() override;

    void blit(void* rgba, int pitch) override;

private:
    void integrate(int thread_index);

    struct PrimitiveHit : GeometryHit {
        const Primitive* primitive;
    };

    std::optional<PrimitiveHit> raycast(const float3& origin, const float3& direction) const;

    Film m_film;
    int m_samples_per_pixel;
    int m_max_diffuse_bounces;
    int m_max_specular_bounces;
    std::vector<Primitive> m_primitives;
    std::vector<Primitive*> m_area_lights;

    int m_thread_count;
    std::vector<std::thread> m_threads;
    std::atomic_bool m_is_running = true;
};
