#include "integrator/bidirectional_path_tracer_integrator.h"
#include "random.h"

#include <algorithm>
#include <cassert>

struct Bounce {
    const Primitive* primitive;
    float3 position;
    float3x3 tangent_space;
    float3x3 inv_tangent_space;
    float3 outgoing_tangent_space;
    float3 beta;
};

BidirectionalPathTracerIntegrator::BidirectionalPathTracerIntegrator(int width, int height, int samples_per_pixel, int max_diffuse_bounces, int max_specular_bounces, std::vector<Primitive>&& primitives)
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
            m_area_lights.push_back(&primitive);
        }
    }

    m_thread_count = std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, m_film.tiles_x * m_film.tiles_y);

    m_threads.reserve(m_thread_count);
    for (int i = 0; i < m_thread_count; i++) {
        m_threads.push_back(std::thread(&BidirectionalPathTracerIntegrator::integrate, this, i));
    }
}

BidirectionalPathTracerIntegrator::~BidirectionalPathTracerIntegrator() {
    m_is_running = false;

    for (std::thread& thread : m_threads) {
        thread.join();
    }
}

void BidirectionalPathTracerIntegrator::blit(void* rgba, int pitch) {
    m_film.blit(rgba, pitch);
}

void BidirectionalPathTracerIntegrator::integrate(int thread_index) {
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

    std::vector<Bounce> camera_bounces;
    camera_bounces.reserve(m_max_diffuse_bounces + m_max_specular_bounces);

    std::vector<Bounce> light_bounces;
    light_bounces.reserve(m_max_diffuse_bounces + m_max_specular_bounces + 1);

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
                double screen_x = x_from + x + random.rand();
                double screen_y = y_from + y + random.rand();

                if (!m_primitives.empty()) {
                    double normalized_x = screen_x * 2.0 / m_film.width - 1.0;
                    double normalized_y = 1.0 - screen_y * 2.0 / m_film.height;

                    float3 origin;
                    float3 outgoing = normalize(point_transform(float3(normalized_x, normalized_y, 1.0), inv_projection));

                    float3 beta(1.0);

                    camera_bounces.clear();

                    int diffuse_bounces = 0;
                    int specular_bounces = 0;

                    while (diffuse_bounces < m_max_diffuse_bounces) {
                        std::optional<PrimitiveHit> hit = raycast(origin, outgoing);
                        if (!hit) {
                            break;
                        }

                        float3x3 tangent_space = transpose(float3x3(hit->tangent, hit->bitangent, hit->normal));
                        float3x3 inverse_tangent_space = inverse(tangent_space);
                        
                        float3 outgoing_tangent_space = normalize((-outgoing) * tangent_space);

                        // TODO: Is this correct?
                        if (!hit->primitive->is_material_specular()) {
                            camera_bounces.push_back(Bounce{ hit->primitive, hit->position, tangent_space, inverse_tangent_space, outgoing_tangent_space, beta });
                        }

                        float3 ingoing_tangent_space;
                        float3 bsdf = hit->primitive->material_bsdf(ingoing_tangent_space, outgoing_tangent_space, random.rand2());
                        if (equal(bsdf, 0.0) || equal(ingoing_tangent_space.z, 0.0)) {
                            break;
                        }

                        origin = hit->position;
                        outgoing = normalize(ingoing_tangent_space * inverse_tangent_space);

                        beta *= bsdf * std::abs(ingoing_tangent_space.z);

                        if (hit->primitive->is_material_specular() && specular_bounces < m_max_specular_bounces) {
                            specular_bounces++;
                        } else {
                            diffuse_bounces++;
                        }
                    }
                }

                if (!m_area_lights.empty()) {
                    int light_index = static_cast<int>(random.rand() * m_area_lights.size());
                    assert(light_index >= 0 && light_index < m_area_lights.size());

                    const Primitive* light = m_area_lights[light_index];
                    assert(light != nullptr);

                    GeometrySample sample = light->geometry_sample(random.rand2());

                    float3x3 tangent_space = transpose(float3x3(sample.tangent, sample.bitangent, sample.normal));
                    float3x3 inverse_tangent_space = inverse(tangent_space);

                    float3 origin = sample.position;

                    float3 outgoing_tangent_space = sample_hemisphere(random.rand2());
                    float3 outgoing = normalize(outgoing_tangent_space * inverse_tangent_space);

                    double pdf_light_index = 1.0 / m_area_lights.size();
                    double pdf_light_sample = 1.0 / light->geometry_area();
                    double pdf_light_outgoing = 1.0 / (2.0 * PI);
                    double pdf_light = pdf_light_index * pdf_light_sample * pdf_light_outgoing;

                    float3 beta = light->material_emissive() * std::abs(outgoing_tangent_space.z) / pdf_light;

                    light_bounces.clear();
                    light_bounces.push_back(Bounce{ light, origin, tangent_space, inverse_tangent_space, float3(0.0, 0.0, 1.0), float3(1.0) });

                    int diffuse_bounces = 0;
                    int specular_bounces = 0;

                    while (diffuse_bounces < m_max_diffuse_bounces) {
                        std::optional<PrimitiveHit> hit = raycast(origin, outgoing);
                        if (!hit) {
                            break;
                        }

                        tangent_space = transpose(float3x3(hit->tangent, hit->bitangent, hit->normal));
                        inverse_tangent_space = inverse(tangent_space);

                        outgoing_tangent_space = normalize((-outgoing) * tangent_space);

                        // TODO: Is this correct?
                        if (!hit->primitive->is_material_specular()) {
                            light_bounces.push_back(Bounce{ hit->primitive, hit->position, tangent_space, inverse_tangent_space, outgoing_tangent_space, beta });
                        }

                        float3 ingoing_tangent_space;
                        float3 bsdf = hit->primitive->material_bsdf(ingoing_tangent_space, outgoing_tangent_space, random.rand2());
                        if (equal(bsdf, 0.0) || equal(ingoing_tangent_space.z, 0.0)) {
                            break;
                        }

                        beta *= bsdf * std::abs(ingoing_tangent_space.z);

                        origin = hit->position;
                        outgoing = normalize(ingoing_tangent_space * inverse_tangent_space);

                        if (hit->primitive->is_material_specular() && specular_bounces < m_max_specular_bounces) {
                            specular_bounces++;
                        } else {
                            diffuse_bounces++;
                        }
                    }
                }

                float3 radiance;

                for (const Bounce& camera_bouce : camera_bounces) {
                    radiance += camera_bouce.beta * camera_bouce.primitive->material_emissive();

                    for (const Bounce& light_bounce : light_bounces) {
                        float3 ingoing = light_bounce.position - camera_bouce.position;
                        double ingoing_length = length(ingoing);
                        ingoing /= ingoing_length;

                        if (!equal(ingoing_length, 0.0)) {
                            float3 camera_bounce_normal = normalize(float3(0.0, 0.0, 1.0) * camera_bouce.inv_tangent_space);
                            float3 light_bounce_normal = normalize(float3(0.0, 0.0, 1.0) * light_bounce.inv_tangent_space);

                            if (dot(camera_bounce_normal, ingoing) > EPSILON && dot(light_bounce_normal, -ingoing) > EPSILON) {
                                std::optional<PrimitiveHit> hit = raycast(camera_bouce.position, ingoing);
                                if (hit && hit->primitive == light_bounce.primitive) {
                                    float3 ingoing_camera_bounce_tangent_space = normalize(ingoing * camera_bouce.tangent_space);
                                    float3 camera_bounce_bsdf = camera_bouce.primitive->material_bsdf(ingoing_camera_bounce_tangent_space, camera_bouce.outgoing_tangent_space);

                                    float3 ingoing_light_bounce_tangent_space = normalize((-ingoing) * light_bounce.tangent_space);
                                    float3 light_bounce_bsdf = light_bounce.primitive->material_bsdf(light_bounce.outgoing_tangent_space, ingoing_light_bounce_tangent_space);

                                    double G = std::abs(ingoing_camera_bounce_tangent_space.z) * std::abs(light_bounce.outgoing_tangent_space.z) / sqr(ingoing_length);
                                    radiance += camera_bouce.beta * camera_bounce_bsdf * (light_bounce_bsdf * light_bounce.beta + light_bounce.primitive->material_emissive()) * G;
                                }
                            }
                        }
                    }
                }

                m_film.add_sample(screen_x, screen_y, radiance);
            }
        }
    }
}

std::optional<BidirectionalPathTracerIntegrator::PrimitiveHit> BidirectionalPathTracerIntegrator::raycast(const float3& origin, const float3& direction) const {
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
