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
    void BounceOffBrick(const SDL_FRect& brick);

    SDL_FRect GetRect() const;
    void Reset();

    void SetSpeedMultiplier(float multiplier);

    bool IsBelowScreen() const;

    // Returns true if the ball hit a wall
    // during its most recent update.
    bool HitWall() const;

private:
    float mX = 0.0f;
    float mY = 0.0f;

    float mSize = 0.0f;

    float mVelocityX = 0.0f;
    float mVelocityY = 0.0f;

    float mSpeed = 0.0f;

    bool mHitWall = false;
};
