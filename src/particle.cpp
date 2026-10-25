#include "particle.hpp"
#include "constants.hpp"

#include <cstdlib>
#include <cmath>

ParticleSystem::ParticleSystem() {
    mParticles.reserve(Constants::MaxParticles);
    for (int i = 0; i < Constants::MaxParticles; ++i) {
        mParticles.emplace_back();
    }
}

void ParticleSystem::Update(float deltaTime) {
    for (auto& p : mParticles) {
        if (!p.active) continue;

        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;
        p.life -= deltaTime;

        if (p.life <= 0.0f) {
            p.active = false;
        }
    }
}

void ParticleSystem::Render(SDL_Renderer* renderer) const {
    for (const auto& p : mParticles) {
        if (!p.active) continue;

        float alpha = 255.0f * (p.life / p.maxLife);
        if (alpha > 255.0f) alpha = 255.0f;
        if (alpha < 0.0f) alpha = 0.0f;

        SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, static_cast<Uint8>(alpha));

        SDL_FRect rect{
            p.x,
            p.y,
            4.0f,
            4.0f
        };

        SDL_RenderFillRect(renderer, &rect);
    }
}

void ParticleSystem::Emit(float x, float y, Uint8 r, Uint8 g, Uint8 b, int count) {
    int emitted = 0;
    for (auto& p : mParticles) {
        if (p.active) continue;
        if (emitted >= count) break;

        p.active = true;
        p.x = x;
        p.y = y;
        p.life = Constants::ParticleLife;
        p.maxLife = Constants::ParticleLife;
        p.r = r;
        p.g = g;
        p.b = b;

        // Random direction
        float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * 3.14159f;
        float speed = 50.0f + static_cast<float>(std::rand()) / RAND_MAX * 150.0f;
        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed;

        ++emitted;
    }
}