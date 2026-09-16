#pragma once

#include <SDL3/SDL.h>

class Brick {
public:
  Brick() = default;

  void Initialize(float x, float y);

  void Render(SDL_Renderer *renderer) const;

  bool IsDestroyed() const;

  void Destroy();

  bool CollidesWith(const SDL_FRect &rect) const;

  SDL_FRect GetRect() const;

private:
  float mX = 0.0f;
  float mY = 0.0f;

  float mWidth = 0.0f;
  float mHeight = 0.0f;

  bool mDestroyed = false;
};