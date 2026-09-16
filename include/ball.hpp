#pragma once

#include <SDL3/SDL.h>

class Ball
{
public:
    Ball() = default;

    void Initialize();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer) const;
    bool CollidesWith(const SDL_FRect& rect) const;
    void BounceOffPaddle(const SDL_FRect& paddle);
    SDL_FRect GetRect() const;
    void Reset();
    bool IsBelowScreen() const;
    void BounceOffBrick(const SDL_FRect& brick);

private:
    float mX = 0.0f;
    float mY = 0.0f;

    float mSize = 0.0f;

    float mVelocityX = 0.0f;
    float mVelocityY = 0.0f;

    float mSpeed = 0.0f;
};
