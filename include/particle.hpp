#pragma once

#include <SDL3/SDL.h>
#include <vector>

struct Particle {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float life = 0.0f;
    float maxLife = 0.0f;
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    bool active = false;
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem() = default;

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer) const;

    void Emit(float x, float y, Uint8 r, Uint8 g, Uint8 b, int count);

private:
    std::vector<Particle> mParticles;
};