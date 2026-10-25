#pragma once

namespace Constants {
constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

constexpr float PaddleWidth = 100.0f;
constexpr float PaddleHeight = 20.0f;
constexpr float PaddleSpeed = 500.0f;

constexpr float PaddleBottomMargin = 40.0f;

constexpr float BallSize = 16.0f;
constexpr float BallSpeed = 350.0f;

constexpr int StartingLives = 3;

constexpr float BrickWidth = 70.0f;
constexpr float BrickHeight = 25.0f;

constexpr float BrickGap = 5.0f;

constexpr int BrickColumns = 10;
constexpr int BrickRows = 5;

constexpr float BrickTopMargin = 60.0f;
constexpr int BrickScore = 100;

// Power-up constants
constexpr float PowerUpSize = 16.0f;
constexpr float PowerUpSpeed = 150.0f;
constexpr int PowerUpChance = 25; // percent chance a brick drops a power-up

constexpr float PowerUpDuration = 10.0f; // seconds

// Particle constants
constexpr int MaxParticles = 200;
constexpr float ParticleLife = 0.8f;

// Level constants
constexpr int MaxLevels = 5;
} // namespace Constants
