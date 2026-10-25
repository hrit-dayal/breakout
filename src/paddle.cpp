#include "paddle.hpp"
#include "constants.hpp"

#include <algorithm>

void Paddle::Initialize()
{
    mWidth  = Constants::PaddleWidth;
    mHeight = Constants::PaddleHeight;
    mSpeed  = Constants::PaddleSpeed;

    mX = (Constants::WindowWidth - mWidth) / 2.0f;

    mY = Constants::WindowHeight
       - Constants::PaddleBottomMargin
       - mHeight;

    mMovingLeft = false;
    mMovingRight = false;
}

void Paddle::Update(float deltaTime)
{
    if (mMovingLeft)
    {
        mX -= mSpeed * deltaTime;
    }

    if (mMovingRight)
    {
        mX += mSpeed * deltaTime;
    }

    // Keep the paddle inside the window.
    mX = std::clamp(
        mX,
        0.0f,
        static_cast<float>(Constants::WindowWidth) - mWidth
    );
}

void Paddle::SetMovingLeft(bool moving)
{
    mMovingLeft = moving;
}

void Paddle::SetMovingRight(bool moving)
{
    mMovingRight = moving;
}

void Paddle::SetWidth(float width)
{
    mWidth = width;
}

void Paddle::Render(SDL_Renderer* renderer) const
{
    SDL_FRect rect{
        mX,
        mY,
        mWidth,
        mHeight
    };

    SDL_SetRenderDrawColor(
        renderer,
        220,
        220,
        220,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &rect
    );
}

SDL_FRect Paddle::GetRect() const
{
    return SDL_FRect{
        mX,
        mY,
        mWidth,
        mHeight
    };
}