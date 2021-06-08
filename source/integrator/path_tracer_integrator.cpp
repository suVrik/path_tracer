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

    for (Primitive& primitive : m_primitives) {
        if (!equal(primitive.material_emissive(), 0.0)) {
            m_light_primitives.push_back(&primitive);
        }
    }

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
    int tiles_per_thread = tiles_total / m_thread_count;
    int threads_with_extra_tile = tiles_total % m_thread_count;

    int begin_tile_index = thread_index * tiles_per_thread + std::min(thread_index, threads_with_extra_tile);
    int end_tile_index = (thread_index + 1) * tiles_per_thread + std::min(thread_index + 1, threads_with_extra_tile);
    int current_tile_index = 0;

    int tiles_count = end_tile_index - begin_tile_index;

    float4x4 projection = float4x4::perspective(radians(30.0), static_cast<double>(m_film.width) / m_film.height, 1.0, 10.0);
    float4x4 inv_projection = inverse(projection);

    while (current_tile_index / tiles_count < m_samples_per_pixel) {
        int temp = current_tile_index++;

        int sample_index = temp / tiles_count;
        int tile_index = begin_tile_index + temp % tiles_count;

        int tile_x = tile_index % m_film.tiles_x;
        int tile_y = tile_index / m_film.tiles_x;

        int x_from = tile_x * TILE_SIZE;
        int y_from = tile_y * TILE_SIZE;

        int tile_width = std::min(x_from + TILE_SIZE, m_film.width) - x_from;
        int tile_height = std::min(y_from + TILE_SIZE, m_film.height) - y_from;

        float3 samples[TILE_SIZE][TILE_SIZE];

        for (int y = 0; y < tile_height; y++) {
            for (int x = 0; x < tile_width; x++) {
                float2 offset = random.rand2();

                double screen_x = x_from + x + offset.x;
                double screen_y = y_from + y + offset.y;
                
                double normalized_x = screen_x * 2.0 / m_film.width - 1.0;
                double normalized_y = 1.0 - screen_y * 2.0 / m_film.height;

                float3 origin(0.0);
                float3 outgoing = normalize(point_transform(float3(normalized_x, normalized_y, 1.0), inv_projection));

                samples[y][x] = sample_ray(random, origin, outgoing, 0, 0);
            }
        }

        m_film.add_samples(tile_x, tile_y, samples);
    }
}

float3 PathTracerIntegrator::sample_ray(Random& random, const float3& origin, const float3& outgoing, int diffuse_bounces, int specular_bounces) {
    std::optional<PrimitiveHit> hit = raycast(origin, outgoing);
    if (!hit) {
        return float3(0.0);
    }

    if (hit->primitive->is_material_specular() && specular_bounces < m_max_specular_bounces) {
        specular_bounces++;
    } else {
        diffuse_bounces++;
    }

    if (diffuse_bounces >= m_max_diffuse_bounces) {
        return hit->primitive->material_emissive();
    }

    float3x3 tangent_space = transpose(float3x3(hit->tangent, hit->bitangent, hit->normal));
    float3x3 inverse_tangent_space = inverse(tangent_space);
                        
    float3 outgoing_tangent_space = normalize((-outgoing) * tangent_space);

    if ((m_light_primitives.empty() || hit->primitive->is_material_specular() || random.rand() < 0.5)) {
        float3 ingoing_tangent_space;
        double material_pdf;
        float3 bsdf = hit->primitive->material_bsdf(ingoing_tangent_space, outgoing_tangent_space, material_pdf, random.rand2());
        if (bsdf == float3(0.0) || ingoing_tangent_space.z == 0.0 || material_pdf == 0.0) {
            return hit->primitive->material_emissive();
        }

        float3 ingoing = normalize(ingoing_tangent_space * inverse_tangent_space);

        double weight = 1.0;
        if (!m_light_primitives.empty() && !hit->primitive->is_material_specular()) {
            double light_pdf = 0.0;
            for (Primitive* light : m_light_primitives) {
                light_pdf += light->geometry_pdf(hit->position, ingoing);
            }
            light_pdf /= m_light_primitives.size();

            weight = sqr(material_pdf) / (sqr(material_pdf) + sqr(light_pdf));
        }

        return hit->primitive->material_emissive() + bsdf * std::abs(ingoing_tangent_space.z) * sample_ray(random, hit->position, ingoing, diffuse_bounces, specular_bounces) * weight / material_pdf;
    } else {
        int light_index = static_cast<int>(random.rand() * m_light_primitives.size());
        assert(m_light_primitives[light_index] != nullptr);

        GeometrySample geometry_sample = m_light_primitives[light_index]->geometry_sample(random.rand2());

        float3 ingoing = normalize(geometry_sample.position - hit->position);
        float3 ingoing_tangent_space = normalize(ingoing * tangent_space);
        if (ingoing_tangent_space.z <= 0.0) {
            return hit->primitive->material_emissive();
        }

        double material_pdf;
        float3 bsdf = hit->primitive->material_bsdf(ingoing_tangent_space, outgoing_tangent_space, material_pdf);
        if (bsdf == float3(0.0) || ingoing_tangent_space.z == 0.0 || material_pdf == 0.0) {
            return hit->primitive->material_emissive();
        }

        double light_pdf = 0.0;
        for (Primitive* light : m_light_primitives) {
            light_pdf += light->geometry_pdf(hit->position, ingoing);
        }
        light_pdf /= m_light_primitives.size();

        double weight = sqr(light_pdf) / (sqr(material_pdf) + sqr(light_pdf));

        return hit->primitive->material_emissive() + bsdf * std::abs(ingoing_tangent_space.z) * sample_ray(random, hit->position, ingoing, diffuse_bounces, specular_bounces) * weight / light_pdf;
    }
}

std::optional<PathTracerIntegrator::PrimitiveHit> PathTracerIntegrator::raycast(const float3& origin, const float3& direction) const {
    std::optional<PrimitiveHit> result;
    double length = std::numeric_limits<double>::infinity();

    for (const Primitive& primitive : m_primitives) {
        std::optional<GeometryHit> hit = primitive.geometry_raycast(origin, direction, length);
        if (hit) {
            result = PrimitiveHit{ *hit, &primitive };
            length = hit->distance;
        }
    }

    return result;
}
