#include "ball.hpp"
#include "constants.hpp"
#include <cmath>

void Ball::Initialize() {
  mSize = Constants::BallSize;
  mSpeed = Constants::BallSpeed;
  Reset();

  // Start near the center of the screen.
  mX = (Constants::WindowWidth - mSize) / 2.0f;

  mY = Constants::WindowHeight / 2.0f;

  // Initial direction: upward and slightly to the right.
  mVelocityX = mSpeed * 0.6f;
  mVelocityY = -mSpeed * 0.8f;
}

void Ball::Update(float deltaTime)
{
    // Reset the collision flag for this frame.
    mHitWall = false;

    mX += mVelocityX * deltaTime;
    mY += mVelocityY * deltaTime;

    // Left wall.
    if (mX <= 0.0f)
    {
        mX = 0.0f;
        mVelocityX = -mVelocityX;
        mHitWall = true;
    }

    // Right wall.
    if (mX + mSize >= Constants::WindowWidth)
    {
        mX = Constants::WindowWidth - mSize;
        mVelocityX = -mVelocityX;
        mHitWall = true;
    }

    // Top wall.
    if (mY <= 0.0f)
    {
        mY = 0.0f;
        mVelocityY = -mVelocityY;
        mHitWall = true;
    }
}

void Ball::Render(SDL_Renderer *renderer) const {
  SDL_FRect rect{mX, mY, mSize, mSize};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_RenderFillRect(renderer, &rect);
}

bool Ball::CollidesWith(const SDL_FRect &rect) const {
  SDL_FRect ballRect{mX, mY, mSize, mSize};

  return SDL_HasRectIntersectionFloat(&ballRect, &rect);
}

void Ball::BounceOffPaddle(const SDL_FRect &paddle) {
  float ballCenter = mX + mSize / 2.0f;

  float paddleCenter = paddle.x + paddle.w / 2.0f;

  float relativePosition = (ballCenter - paddleCenter) / (paddle.w / 2.0f);

  // Keep the value within [-1, 1].
  if (relativePosition < -1.0f) {
    relativePosition = -1.0f;
  }

  if (relativePosition > 1.0f) {
    relativePosition = 1.0f;
  }

  // Horizontal velocity is determined
  // by where the ball hits the paddle.
  mVelocityX = relativePosition * mSpeed;

  // Make sure the ball always travels upward.
  float verticalSpeed = mSpeed * mSpeed - mVelocityX * mVelocityX;

  mVelocityY = -std::sqrt(verticalSpeed);

  // Move the ball above the paddle so that
  // we don't immediately collide again.
  mY = paddle.y - mSize;
}

SDL_FRect Ball::GetRect() const { return SDL_FRect{mX, mY, mSize, mSize}; }

void Ball::SetSpeedMultiplier(float multiplier)
{
    mVelocityX *= multiplier;
    mVelocityY *= multiplier;
    mSpeed *= multiplier;
}

void Ball::Reset() {
  mX = (Constants::WindowWidth - mSize) / 2.0f;

  mY = Constants::WindowHeight / 2.0f;

  mVelocityX = mSpeed * 0.6f;
  mVelocityY = -mSpeed * 0.8f;
}

bool Ball::IsBelowScreen() const { return mY > Constants::WindowHeight; }

void Ball::BounceOffBrick(const SDL_FRect &brick) {
  SDL_FRect ball = GetRect();

  float ballCenterX = ball.x + ball.w / 2.0f;
  float ballCenterY = ball.y + ball.h / 2.0f;

  float brickCenterX = brick.x + brick.w / 2.0f;
  float brickCenterY = brick.y + brick.h / 2.0f;

  float dx = ballCenterX - brickCenterX;
  float dy = ballCenterY - brickCenterY;

  float overlapX = (ball.w / 2.0f + brick.w / 2.0f) - std::abs(dx);

  float overlapY = (ball.h / 2.0f + brick.h / 2.0f) - std::abs(dy);

  if (overlapX < overlapY) {
    mVelocityX = -mVelocityX;

    if (dx > 0.0f) {
      mX = brick.x + brick.w;
    } else {
      mX = brick.x - mSize;
    }
  } else {
    mVelocityY = -mVelocityY;

    if (dy > 0.0f) {
      mY = brick.y + brick.h;
    } else {
      mY = brick.y - mSize;
    }
  }
}

bool Ball::HitWall() const
{
    return mHitWall;
}
