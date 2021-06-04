#include "integrator/path_tracer_integrator.h"
#include "random.h"

#include <algorithm>
#include <cassert>

PathTracerIntegrator::PathTracerIntegrator(int width, int height, int samples_per_pixel, int max_diffuse_bounces, int max_specular_bounces, std::vector<Primitive>&& primitives)
    : m_film(width, height)
    , m_samples_per_pixel(samples_per_pixel)
    , m_max_diffuse_bounces(max_diffuse_bounces)
    , m_max_specular_bounces(max_specular_bounces)
    , m_primitives(std::move(primitives))
{
    assert(m_samples_per_pixel > 0);
    assert(m_max_diffuse_bounces > 0);
    assert(m_max_specular_bounces >= 0);

    m_thread_count = std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, m_film.tiles_x * m_film.tiles_y);

    m_threads.reserve(m_thread_count);
    for (int i = 0; i < m_thread_count; i++) {
        m_threads.push_back(std::thread(&PathTracerIntegrator::integrate, this, i));
    }
}

PathTracerIntegrator::~PathTracerIntegrator() {
    for (std::thread& thread : m_threads) {
        thread.join();
    }
}

void PathTracerIntegrator::blit(void* rgba, int pitch) {
    m_film.blit(rgba, pitch);
}

void PathTracerIntegrator::integrate(int thread_index) {
    assert(thread_index >= 0 && thread_index < m_thread_count);

    Random random(thread_index);

    int tiles_total = m_film.tiles_x * m_film.tiles_y;
    int tiles_per_thread = (tiles_total + m_thread_count - 1) / m_thread_count;

    int begin_tile_index = thread_index * tiles_per_thread;
    int end_tile_index = std::min(tiles_total, (thread_index + 1) * tiles_per_thread);
    int current_tile_index = 0;

    int tiles_count = end_tile_index - begin_tile_index;

    float4x4 projection = float4x4::perspective(radians(30.f), static_cast<float>(m_film.width) / m_film.height, 1.f, 10.f);
    float4x4 inv_projection = inverse(projection);

    while (current_tile_index / tiles_count < m_samples_per_pixel) {
        int tile_index = begin_tile_index + (current_tile_index++) % tiles_count;

        int tile_x = tile_index % m_film.tiles_x;
        int tile_y = tile_index / m_film.tiles_x;

        int x_from = tile_x * TILE_SIZE;
        int y_from = tile_y * TILE_SIZE;

        int tile_width = std::min(x_from + TILE_SIZE, m_film.width) - x_from;
        int tile_height = std::min(y_from + TILE_SIZE, m_film.height) - y_from;

        for (int y = 0; y < tile_height; y++) {
            for (int x = 0; x < tile_width; x++) {
                float screen_x = x_from + x + random.rand();
                float screen_y = y_from + y + random.rand();
                
                float normalized_x = screen_x * 2.f / m_film.width - 1.f;
                float normalized_y = 1.f - screen_y * 2.f / m_film.height;

                float3 origin(0.f);
                float3 outgoing = normalize(point_transform(float3(normalized_x, normalized_y, 1.f), inv_projection));

                float3 beta(1.f);
                float3 radiance(0.f);

                int diffuse_bounces = 0;
                int specular_bounces = 0;

                while (diffuse_bounces < m_max_diffuse_bounces) {
                    std::optional<PrimitiveHit> hit = raycast(origin, outgoing);
                    if (!hit) {
                        break;
                    }

                    radiance += beta * hit->primitive->material_emissive();

                    float3x3 tangent_space = transpose(float3x3(hit->tangent, hit->bitangent, hit->normal));
                    float3x3 inverse_tangent_space = inverse(tangent_space);
                        
                    float3 outgoing_tangent_space = normalize((-outgoing) * tangent_space);

                    float3 ingoing_tangent_space;
                    float3 bsdf = hit->primitive->material_bsdf(ingoing_tangent_space, outgoing_tangent_space, random.rand2());
                    if (equal(bsdf, 0.f) || equal(ingoing_tangent_space.z, 0.f)) {
                        break;
                    }

                    origin = hit->position;
                    outgoing = normalize(ingoing_tangent_space * inverse_tangent_space);

                    beta *= bsdf * std::abs(ingoing_tangent_space.z) * 2;
                    if (equal(beta, 0.f)) {
                        break;
                    }

                    if (hit->primitive->is_material_specular() && specular_bounces < m_max_specular_bounces) {
                        specular_bounces++;
                    } else {
                        diffuse_bounces++;
                    }
                }

                m_film.add_sample(screen_x, screen_y, radiance);
            }
        }
    }
}

std::optional<PathTracerIntegrator::PrimitiveHit> PathTracerIntegrator::raycast(const float3& origin, const float3& direction) const {
    std::optional<PrimitiveHit> result;
    float length = std::numeric_limits<float>::infinity();

    for (const Primitive& primitive : m_primitives) {
        std::optional<GeometryHit> hit = primitive.geometry_raycast(origin, direction, length);
        if (hit) {
            result = PrimitiveHit{ *hit, &primitive };
            length = hit->distance;
        }
    }

    return result;
}
