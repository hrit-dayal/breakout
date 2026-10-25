#pragma once

#include <SDL3/SDL.h>

class Paddle
{
public:
    Paddle() = default;

    void Initialize();

    void Update(float deltaTime);

    void Render(SDL_Renderer* renderer) const;

    void SetMovingLeft(bool moving);
    void SetMovingRight(bool moving);

    void SetWidth(float width);

    SDL_FRect GetRect() const;

private:
    float mX = 0.0f;
    float mY = 0.0f;

    float mWidth = 0.0f;
    float mHeight = 0.0f;

    float mSpeed = 0.0f;

    bool mMovingLeft = false;
    bool mMovingRight = false;
};
