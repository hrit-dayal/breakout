#include "brick.hpp"
#include "constants.hpp"

void Brick::Initialize(float x, float y) {
  mX = x;
  mY = y;

  mWidth = Constants::BrickWidth;
  mHeight = Constants::BrickHeight;

  mDestroyed = false;
}

void Brick::Render(SDL_Renderer *renderer) const {
  if (mDestroyed) {
    return;
  }

  SDL_FRect rect{mX, mY, mWidth, mHeight};

  SDL_SetRenderDrawColor(renderer, 200, 80, 80, 255);

  SDL_RenderFillRect(renderer, &rect);
}

bool Brick::IsDestroyed() const { return mDestroyed; }

void Brick::Destroy() { mDestroyed = true; }

bool Brick::CollidesWith(const SDL_FRect &rect) const {
  if (mDestroyed) {
    return false;
  }

  SDL_FRect brickRect{mX, mY, mWidth, mHeight};

  return SDL_HasRectIntersectionFloat(&brickRect, &rect);
}

SDL_FRect Brick::GetRect() const { return SDL_FRect{mX, mY, mWidth, mHeight}; }