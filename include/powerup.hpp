#pragma once

#include <SDL3/SDL.h>

enum class PowerUpType {
    WidePaddle,
    MultiBall,
    SlowBall,
    ExtraLife
};

class PowerUp {
public:
    PowerUp() = default;

    void Initialize(float x, float y, PowerUpType type);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer) const;

    bool IsActive() const { return mActive; }
    void Deactivate() { mActive = false; }

    SDL_FRect GetRect() const;
    PowerUpType GetType() const { return mType; }
    bool IsBelowScreen() const;

private:
    float mX = 0.0f;
    float mY = 0.0f;
    float mSize = 0.0f;
    bool mActive = false;
    PowerUpType mType = PowerUpType::WidePaddle;
};