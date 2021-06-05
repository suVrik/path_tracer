#include "geometry/box_geometry.h"
#include "geometry/sphere_geometry.h"
#include "integrator/bidirectional_path_tracer_integrator.h"
#include "integrator/path_tracer_integrator.h"
#include "material/diffuse_material.h"
#include "material/emissive_material.h"
#include "material/specular_reflective_material.h"
#include "material/specular_transmissive_material.h"
#include "primitive.h"

#include <cassert>
#include <SDL2/SDL.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int TEXTURE_WIDTH = 400;
constexpr int TEXTURE_HEIGHT = 300;
constexpr int SAMPLES_PER_PIXEL = 16384;
constexpr int DIFFUSE_BOUNCES_MAX = 4;
constexpr int SPECULAR_BOUNCES_MAX = 4;

static std::vector<Primitive> build_scene() {
    auto long_wall_geometry = std::make_shared<BoxGeometry>(float3(0.5, 0.5, 1.5));
    assert(long_wall_geometry != nullptr);
    
    auto short_wall_geometry = std::make_shared<BoxGeometry>(float3(0.5));
    assert(short_wall_geometry != nullptr);

    auto top_side_geometry = std::make_shared<BoxGeometry>(float3(0.2, 0.5, 1.5));
    assert(top_side_geometry != nullptr);

    auto top_front_geometry = std::make_shared<BoxGeometry>(float3(0.1, 0.5, 0.2));
    assert(top_front_geometry != nullptr);

    auto top_back_geometry = std::make_shared<BoxGeometry>(float3(0.1, 0.5, 1.2));
    assert(top_back_geometry != nullptr);
    
    auto lamp_geometry = std::make_shared<BoxGeometry>(float3(0.1));
    assert(lamp_geometry != nullptr);

    auto left_box_geometry = std::make_shared<BoxGeometry>(float3(0.15, 0.275, 0.15));
    assert(left_box_geometry != nullptr);

    auto right_box_geometry = std::make_shared<BoxGeometry>(float3(0.15));
    assert(right_box_geometry != nullptr);

    auto sphere_geometry = std::make_shared<SphereGeometry>(0.15);
    assert(sphere_geometry != nullptr);

    auto red_material = std::make_shared<DiffuseMaterial>(float3(1.0, 0.0, 0.0));
    assert(red_material != nullptr);
    
    auto green_material = std::make_shared<DiffuseMaterial>(float3(0.0, 1.0, 0.0));
    assert(green_material != nullptr);
    
    auto white_material = std::make_shared<DiffuseMaterial>(float3(1.0));
    assert(white_material != nullptr);

    auto emissive_material = std::make_shared<EmissiveMaterial>(float3(3.0));
    assert(emissive_material != nullptr);

    auto reflective_material = std::make_shared<SpecularReflectiveMaterial>(10.0);
    assert(reflective_material != nullptr);

    auto transmissive_material = std::make_shared<SpecularTransmissiveMaterial>(2.5);
    assert(transmissive_material != nullptr);

    float4x4 left_box_transform = float4x4::rotation(float3(0.0, 1.0, 0.0), radians(-20.0)) * float4x4::translation(float3(-0.175, -0.225, 2.5));
    float4x4 right_box_transform = float4x4::rotation(float3(0.0, 1.0, 0.0), radians(20.0)) * float4x4::translation(float3(0.175, -0.35, 2.3));

    return std::vector<Primitive> {
        { long_wall_geometry,  red_material,   float4x4::translation(float3(-1.0, 0.0, 1.4))  }, // left wall
        { long_wall_geometry,  green_material, float4x4::translation(float3(1.0, 0.0, 1.4))   }, // right wall
        { long_wall_geometry,  white_material, float4x4::translation(float3(0.0, -1.0, 1.4))  }, // bottom wall
        { short_wall_geometry, white_material, float4x4::translation(float3(0.0, 0.0, 3.4))   }, // front wall
        { short_wall_geometry, white_material, float4x4::translation(float3(0.0, 0.0, -0.6))  }, // back wall
        { top_side_geometry,   white_material, float4x4::translation(float3(-0.3, 1.0, 1.4)) }, // top left wall
        { top_side_geometry,   white_material, float4x4::translation(float3(0.3, 1.0, 1.4))  }, // top right wall
        { top_front_geometry,  white_material, float4x4::translation(float3(0.0, 1.0, 2.7))   }, // top front wall
        { top_back_geometry,   white_material, float4x4::translation(float3(0.0, 1.0, 1.1))   }, // top back wall

        { left_box_geometry,  white_material, left_box_transform  }, // left box
        { right_box_geometry, white_material, right_box_transform }, // right box

        //{ sphere_geometry, reflective_material, float4x4::translation(float3(-0.175, -0.35, 2.5)) }, // left sphere
        //{ sphere_geometry, reflective_material, float4x4::translation(float3(0.175, -0.35, 2.3)) }, // right sphere

        //{ sphere_geometry, transmissive_material, float4x4::translation(float3(-0.175, -0.35, 2.5)) }, // left sphere
        //{ sphere_geometry, transmissive_material, float4x4::translation(float3(0.175, -0.35, 2.3)) }, // right sphere

        { lamp_geometry, emissive_material, float4x4::translation(float3(0.0, 0.61, 2.4)) }, // lamp
    };
}

static bool poll_events() {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event) != 0) {
        if (sdl_event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

static void blit(Integrator* integrator, SDL_Texture* texture) {
    void* data;
    int pitch;

    int lock_texture = SDL_LockTexture(texture, NULL, &data, &pitch);
    assert(lock_texture == 0);

    integrator->blit(data, pitch);

    SDL_UnlockTexture(texture);
}

static void present(SDL_Renderer* renderer, SDL_Texture* texture) {
    int render_copy = SDL_RenderCopy(renderer, texture, NULL, NULL);
    assert(render_copy == 0);

    SDL_RenderPresent(renderer);
}

int main(int arc, char* argv[]) {
    int init = SDL_Init(SDL_INIT_VIDEO);
    assert(init == 0);

    SDL_Window* window = SDL_CreateWindow("Path Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    assert(window != nullptr);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    assert(renderer != nullptr);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    assert(texture != nullptr);

    auto integrator = std::make_unique<PathTracerIntegrator>(TEXTURE_WIDTH, TEXTURE_HEIGHT, SAMPLES_PER_PIXEL, DIFFUSE_BOUNCES_MAX, SPECULAR_BOUNCES_MAX, build_scene());
    assert(integrator != nullptr);

    while (poll_events()) {
        blit(integrator.get(), texture);
        present(renderer, texture);
    }

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
