#include "powerup.hpp"
#include "constants.hpp"

#include <cstdlib>

void PowerUp::Initialize(float x, float y, PowerUpType type) {
    mX = x;
    mY = y;
    mSize = Constants::PowerUpSize;
    mActive = true;
    mType = type;
}

void PowerUp::Update(float deltaTime) {
    if (!mActive) return;

    mY += Constants::PowerUpSpeed * deltaTime;
}

void PowerUp::Render(SDL_Renderer* renderer) const {
    if (!mActive) return;

    SDL_FRect rect{mX, mY, mSize, mSize};

    // Color based on type
    switch (mType) {
        case PowerUpType::WidePaddle:
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);   // Green
            break;
        case PowerUpType::MultiBall:
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan
            break;
        case PowerUpType::SlowBall:
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
            break;
        case PowerUpType::ExtraLife:
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Magenta
            break;
    }

    SDL_RenderFillRect(renderer, &rect);
}

SDL_FRect PowerUp::GetRect() const {
    return SDL_FRect{mX, mY, mSize, mSize};
}

bool PowerUp::IsBelowScreen() const {
    return mY > Constants::WindowHeight;
}