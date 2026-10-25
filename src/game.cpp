#include "game.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <cstdlib>

bool Game::Initialize() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    return false;
  }

  if (!TTF_Init()) {
    SDL_Log("TTF_Init failed: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  mWindow = SDL_CreateWindow("Breakout", Constants::WindowWidth,
                             Constants::WindowHeight, SDL_WINDOW_RESIZABLE);

  if (!mWindow) {
    SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());

    TTF_Quit();
    SDL_Quit();

    return false;
  }

  mRenderer = SDL_CreateRenderer(mWindow, nullptr);

  if (!mRenderer) {
    SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());

    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;

    TTF_Quit();
    SDL_Quit();

    return false;
  }

  std::string fontPath = GetAssetPath("assets/fonts/ARIAL.TTF");

  if (!mScoreText.Initialize(mRenderer, fontPath.c_str(), 24.0f)) {
    Shutdown();
    return false;
  }

  if (!mLivesText.Initialize(mRenderer, fontPath.c_str(), 24.0f)) {
    Shutdown();
    return false;
  }

  if (!mMenu.Initialize(mRenderer, fontPath.c_str())) {
    Shutdown();
    return false;
  }

  if (!mOptionsMenu.Initialize(mRenderer, fontPath.c_str())) {
    Shutdown();
    return false;
  }

  if (!mStatusText.Initialize(mRenderer, fontPath.c_str(), 32.0f)) {
    Shutdown();
    return false;
  }

  if (!mHighScoreText.Initialize(mRenderer, fontPath.c_str(), 24.0f)) {
    Shutdown();
    return false;
  }

  if (!mLevelText.Initialize(mRenderer, fontPath.c_str(), 24.0f)) {
    Shutdown();
    return false;
  }

  const std::string paddlePath = GetAssetPath("assets/audio/paddle.wav");

  const std::string brickPath = GetAssetPath("assets/audio/brick.wav");

  const std::string wallPath = GetAssetPath("assets/audio/wall.wav");

  const std::string menuPath = GetAssetPath("assets/audio/menu.wav");

  const std::string gameOverPath = GetAssetPath("assets/audio/gameover.wav");
  const std::string victoryPath = GetAssetPath("assets/audio/victory.wav");
  const std::string losePath = GetAssetPath("assets/audio/lose.wav");
  const std::string musicPath = GetAssetPath("assets/audio/music.ogg");

  if (!mAudio.Initialize(paddlePath.c_str(), brickPath.c_str(),
                         wallPath.c_str(), menuPath.c_str(),
                         gameOverPath.c_str(), victoryPath.c_str(),
                         losePath.c_str(), musicPath.c_str())) {
    Shutdown();
    return false;
  }

  mScoreText.SetText("Score: 0");
  mLivesText.SetText("Lives: 3");

  mHighScore.Load();
  UpdateHighScoreText();

  mMenuSelection = 0;

  mLives = Constants::StartingLives;
  mScore = 0;

  mState = GameState::Menu;
  mRunning = true;

  return true;
}

void Game::Run() {
  Uint64 previousTime = SDL_GetTicks();

  while (mRunning) {
    Uint64 currentTime = SDL_GetTicks();

    float deltaTime = static_cast<float>(currentTime - previousTime) / 1000.0f;

    previousTime = currentTime;

    if (deltaTime > 0.1f) {
      deltaTime = 0.1f;
    }

    ProcessEvents();
    Update(deltaTime);
    Render();
  }
}

void Game::ProcessEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:

      mRunning = false;

      break;

    case SDL_EVENT_KEY_DOWN:

      if (event.key.key == SDLK_Q) {
        mRunning = false;
        break;
      }

      if (mState == GameState::Menu) {
        if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN) {
          mMenu.HandleKey(event.key.key);

          // Play sound when moving through menu items.
          mAudio.PlayMenu();
        } else if (event.key.key == SDLK_RETURN) {
          // Play sound when confirming a menu selection.
          mAudio.PlayMenu();

          switch (mMenu.GetSelection()) {
          case 0:
            StartGame();
            break;

          case 1:
            mOptionsMenu.ResetSelection();
            mState = GameState::Options;
            break;

          case 2:
            mRunning = false;
            break;
          }
        } else if (event.key.key == SDLK_ESCAPE) {
          mRunning = false;
        }

        break;
      }

      if (mState == GameState::Options) {
        if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN) {
          mOptionsMenu.HandleKey(event.key.key);

          mAudio.PlayMenu();
        } else if (event.key.key == SDLK_RETURN) {
          int selection = mOptionsMenu.GetSelection();

          if (selection == 2) {
            // BACK
            mAudio.PlayMenu();

            mMenu.ResetSelection();
            mState = GameState::Menu;
          } else {
            // Toggle Sound or Music.
            mOptionsMenu.HandleKey(SDLK_RETURN);

            // Apply the Sound setting to the audio system.
            mAudio.SetEnabled(mOptionsMenu.IsSoundEnabled());
            // Enable music
            mAudio.SetMusicEnabled(mOptionsMenu.IsMusicEnabled());

            // Plays only if sound is now enabled.
            mAudio.PlayMenu();
          }
        } else if (event.key.key == SDLK_ESCAPE) {
          mAudio.PlayMenu();

          mMenu.ResetSelection();
          mState = GameState::Menu;
        }

        break;
      }

      if (event.key.key == SDLK_ESCAPE) {
        if (mState == GameState::Playing) {
          mState = GameState::Paused;
        } else if (mState == GameState::Paused) {
          mState = GameState::Playing;
        }
      }

      if (event.key.key == SDLK_RETURN) {
        if (mState == GameState::Paused) {
          mState = GameState::Playing;
        } else if (mState == GameState::GameOver) {
          StartGame();
        } else if (mState == GameState::Victory) {
          StartGame();
        }
      }

      if (mState == GameState::Playing) {
        if (event.key.key == SDLK_LEFT) {
          mPaddle.SetMovingLeft(true);
        }

        if (event.key.key == SDLK_RIGHT) {
          mPaddle.SetMovingRight(true);
        }
      }

      break;

    case SDL_EVENT_KEY_UP:

      if (event.key.key == SDLK_LEFT) {
        mPaddle.SetMovingLeft(false);
      }

      if (event.key.key == SDLK_RIGHT) {
        mPaddle.SetMovingRight(false);
      }

      break;

    default:
      break;
    }
  }
}

void Game::Update(float deltaTime) {
  switch (mState) {
  case GameState::Menu:
    break;

  case GameState::Options:
    break;

  case GameState::Playing:
    UpdatePlaying(deltaTime);
    break;

  case GameState::Paused:
    break;

  case GameState::GameOver:
    break;

  case GameState::Victory:
    break;
  }
}

void Game::UpdatePlaying(float deltaTime) {
  mPaddle.Update(deltaTime);
  mParticleSystem.Update(deltaTime);
  UpdatePowerUps(deltaTime);
  UpdatePowerUpTimers(deltaTime);

  // Screen shake decay
  if (mScreenShakeTimer > 0.0f) {
    mScreenShakeTimer -= deltaTime;
    if (mScreenShakeTimer < 0.0f) {
      mScreenShakeTimer = 0.0f;
      mScreenShakeIntensity = 0.0f;
    }
  }

  // Update all balls
  std::vector<Ball> activeBalls;
  for (auto& ball : mBalls) {
    ball.Update(deltaTime);

    if (ball.HitWall()) {
      mAudio.PlayWall();
      mScreenShakeTimer = 0.1f;
      mScreenShakeIntensity = 4.0f;
    }

    // Ball-paddle collision
    SDL_FRect paddleRect = mPaddle.GetRect();
    if (ball.CollidesWith(paddleRect)) {
      ball.BounceOffPaddle(paddleRect);
      mAudio.PlayPaddle();
      mScreenShakeTimer = 0.05f;
      mScreenShakeIntensity = 2.0f;
    }

    // Ball-brick collisions
    for (Brick& brick : mBricks) {
      if (brick.IsDestroyed()) continue;

      if (ball.CollidesWith(brick.GetRect())) {
        ball.BounceOffBrick(brick.GetRect());
        mAudio.PlayBrick();
        brick.Destroy();

        mScore += Constants::BrickScore;
        char scoreText[64];
        SDL_snprintf(scoreText, sizeof(scoreText), "Score: %d", mScore);
        mScoreText.SetText(scoreText);

        // Particles at brick position
        SDL_FRect brickRect = brick.GetRect();
        mParticleSystem.Emit(
            brickRect.x + brickRect.w / 2.0f,
            brickRect.y + brickRect.h / 2.0f,
            200, 80, 80, 12);

        // Chance to spawn power-up
        SpawnPowerUp(
            brickRect.x + brickRect.w / 2.0f,
            brickRect.y + brickRect.h / 2.0f);

        break; // One brick collision per ball per frame
      }
    }

    // Keep ball if still in play
    if (!ball.IsBelowScreen()) {
      activeBalls.push_back(ball);
    }
  }
  mBalls = std::move(activeBalls);

  // Check for all balls lost
  if (mBalls.empty()) {
    mAudio.PlayLose();
    --mLives;

    char livesText[64];
    SDL_snprintf(livesText, sizeof(livesText), "Lives: %d", mLives);
    mLivesText.SetText(livesText);

    if (mLives > 0) {
      mBalls.clear();
      Ball ball;
      ball.Initialize();
      mBalls.push_back(ball);
    } else {
      mAudio.PlayGameOver();
      EndRound(GameState::GameOver);
      return;
    }
  }

  // Check victory / level complete
  if (AreAllBricksDestroyed()) {
    mAudio.PlayVictory();
    if (mCurrentLevel >= Constants::MaxLevels) {
      EndRound(GameState::Victory);
    } else {
      NextLevel();
    }
    return;
  }
}

void Game::Render() {
  // Screen shake offset
  float shakeX = 0.0f;
  float shakeY = 0.0f;
  if (mScreenShakeTimer > 0.0f) {
    shakeX = ((static_cast<float>(std::rand()) / RAND_MAX) * 2.0f - 1.0f) *
             mScreenShakeIntensity;
    shakeY = ((static_cast<float>(std::rand()) / RAND_MAX) * 2.0f - 1.0f) *
             mScreenShakeIntensity;
  }

  SDL_SetRenderDrawColor(mRenderer, 10, 10, 20, 255);

  SDL_RenderClear(mRenderer);

  if (mState == GameState::Playing || mState == GameState::Paused) {
    for (const Brick& brick : mBricks) {
      brick.Render(mRenderer);
    }

    for (const PowerUp& powerUp : mPowerUps) {
      powerUp.Render(mRenderer);
    }

    mPaddle.Render(mRenderer);
    for (const Ball& ball : mBalls) {
      ball.Render(mRenderer);
    }

    mParticleSystem.Render(mRenderer);

    mScoreText.Render(20.0f + shakeX, 15.0f + shakeY);
    mLivesText.Render(680.0f + shakeX, 15.0f + shakeY);
    mHighScoreText.Render(
        (Constants::WindowWidth - mHighScoreText.GetWidth()) / 2.0f + shakeX,
        15.0f + shakeY);
    mLevelText.Render(
        (Constants::WindowWidth - mLevelText.GetWidth()) / 2.0f + shakeX,
        50.0f + shakeY);
  }

  RenderStateText();

  SDL_RenderPresent(mRenderer);
}

void Game::RenderStateText() {
  switch (mState) {
  case GameState::Menu: {
    mMenu.Render(mRenderer);

    mHighScoreText.Render(
        (Constants::WindowWidth - mHighScoreText.GetWidth()) / 2.0f, 440.0f);

    break;
  }

  case GameState::Options:

    mOptionsMenu.Render(mRenderer);

    break;

  case GameState::Paused:

    mStatusText.SetText("PAUSED - Press ENTER to Continue");

    mStatusText.Render(170.0f, 280.0f);

    break;

  case GameState::GameOver:

    mStatusText.SetText("GAME OVER - Press ENTER to Restart");

    mStatusText.Render(140.0f, 280.0f);

    mHighScoreText.Render(
        (Constants::WindowWidth - mHighScoreText.GetWidth()) / 2.0f, 330.0f);

    break;

  case GameState::Victory:

    mStatusText.SetText("YOU WIN - Press ENTER to Restart");

    mStatusText.Render(160.0f, 280.0f);

    mHighScoreText.Render(
        (Constants::WindowWidth - mHighScoreText.GetWidth()) / 2.0f, 330.0f);

    break;

  case GameState::Playing:

    break;
  }
}

void Game::CreateBricks() {
  mBricks.clear();

  const float totalWidth = Constants::BrickColumns * Constants::BrickWidth +
                           (Constants::BrickColumns - 1) * Constants::BrickGap;

  const float startX = (Constants::WindowWidth - totalWidth) / 2.0f;

  for (int row = 0; row < Constants::BrickRows; ++row) {
    for (int column = 0; column < Constants::BrickColumns; ++column) {
      float x = startX + column * (Constants::BrickWidth + Constants::BrickGap);

      float y = Constants::BrickTopMargin +
                row * (Constants::BrickHeight + Constants::BrickGap);

      // Level-based patterns: skip some bricks for variety
      bool skip = false;
      if (mCurrentLevel == 2 && (row + column) % 3 == 0) {
        skip = true;
      } else if (mCurrentLevel == 3 && row % 2 == 0 && column % 2 == 0) {
        skip = true;
      } else if (mCurrentLevel == 4 && (row == 0 || row == Constants::BrickRows - 1) &&
                 (column == 0 || column == Constants::BrickColumns - 1)) {
        skip = true; // hollow rectangle
      } else if (mCurrentLevel >= 5 && (row + column) % 2 == 0) {
        skip = true; // checkerboard
      }

      if (skip) continue;

      Brick brick;
      brick.Initialize(x, y);

      mBricks.push_back(brick);
    }
  }
}

void Game::StartGame() {
  mLives = Constants::StartingLives;
  mScore = 0;
  mCurrentLevel = 1;

  mPaddle.Initialize();
  mBalls.clear();
  Ball ball;
  ball.Initialize();
  mBalls.push_back(ball);

  mPowerUps.clear();
  mWidePaddleTimer = 0.0f;
  mSlowBallTimer = 0.0f;
  mPaddle = Paddle(); // reset paddle width
  mPaddle.Initialize();

  CreateBricks();

  mScoreText.SetText("Score: 0");
  mLivesText.SetText("Lives: 3");
  mLevelText.SetText("Level: 1");

  mState = GameState::Playing;
}

void Game::NextLevel() {
  ++mCurrentLevel;

  mPaddle.Initialize();
  mBalls.clear();
  Ball ball;
  ball.Initialize();
  mBalls.push_back(ball);

  mPowerUps.clear();
  mWidePaddleTimer = 0.0f;
  mSlowBallTimer = 0.0f;

  CreateBricks();

  char levelText[64];
  SDL_snprintf(levelText, sizeof(levelText), "Level: %d", mCurrentLevel);
  mLevelText.SetText(levelText);

  mState = GameState::Playing;
}

bool Game::AreAllBricksDestroyed() const {
  for (const Brick &brick : mBricks) {
    if (!brick.IsDestroyed()) {
      return false;
    }
  }

  return true;
}

void Game::EndRound(GameState finalState) {
  mHighScore.SubmitScore(mScore);
  UpdateHighScoreText();

  mState = finalState;
}

void Game::UpdateHighScoreText() {
  char buffer[64];

  SDL_snprintf(buffer, sizeof(buffer), "High Score: %d", mHighScore.GetScore());

  mHighScoreText.SetText(buffer);
}

void Game::SpawnPowerUp(float x, float y) {
  if ((std::rand() % 100) >= Constants::PowerUpChance) {
    return;
  }

  int typeIndex = std::rand() % 4;
  PowerUpType type = static_cast<PowerUpType>(typeIndex);

  PowerUp powerUp;
  powerUp.Initialize(x, y, type);
  mPowerUps.push_back(powerUp);
}

void Game::UpdatePowerUps(float deltaTime) {
  for (auto& powerUp : mPowerUps) {
    if (!powerUp.IsActive()) continue;
    powerUp.Update(deltaTime);
  }

  // Remove inactive power-ups
  mPowerUps.erase(
      std::remove_if(mPowerUps.begin(), mPowerUps.end(),
                     [](const PowerUp& p) { return !p.IsActive(); }),
      mPowerUps.end());

  // Check paddle collisions
  SDL_FRect paddleRect = mPaddle.GetRect();
  for (auto& powerUp : mPowerUps) {
    if (!powerUp.IsActive()) continue;

    SDL_FRect powerUpRect = powerUp.GetRect();
    if (SDL_HasRectIntersectionFloat(&powerUpRect, &paddleRect)) {
      ApplyPowerUp(powerUp);
      powerUp.Deactivate();
    }
  }

  // Remove power-ups that fell off screen
  mPowerUps.erase(
      std::remove_if(mPowerUps.begin(), mPowerUps.end(),
                     [](const PowerUp& p) { return p.IsBelowScreen(); }),
      mPowerUps.end());
}

void Game::ApplyPowerUp(const PowerUp& powerUp) {
  switch (powerUp.GetType()) {
    case PowerUpType::WidePaddle: {
      mWidePaddleTimer = Constants::PowerUpDuration;
      mPaddle.SetWidth(Constants::PaddleWidth * 2.0f);
      break;
    }
    case PowerUpType::MultiBall: {
      // Spawn 2 additional balls with varied velocity
      for (int i = 0; i < 2; ++i) {
        Ball newBall;
        newBall.Initialize();
        newBall = mBalls[0];
        // Add variance to velocity so balls don't overlap
        float variance = (static_cast<float>(std::rand()) / RAND_MAX * 2.0f - 1.0f) * 0.3f;
        newBall.SetSpeedMultiplier(1.0f + variance);
        mBalls.push_back(newBall);
      }
      break;
    }
    case PowerUpType::SlowBall: {
      mSlowBallTimer = Constants::PowerUpDuration;
      for (auto& ball : mBalls) {
        ball.SetSpeedMultiplier(0.5f);
      }
      break;
    }
    case PowerUpType::ExtraLife: {
      ++mLives;
      char livesText[64];
      SDL_snprintf(livesText, sizeof(livesText), "Lives: %d", mLives);
      mLivesText.SetText(livesText);
      break;
    }
  }
}

void Game::UpdatePowerUpTimers(float deltaTime) {
  if (mWidePaddleTimer > 0.0f) {
    mWidePaddleTimer -= deltaTime;
    if (mWidePaddleTimer <= 0.0f) {
      mWidePaddleTimer = 0.0f;
      mPaddle.SetWidth(Constants::PaddleWidth);
    }
  }

  if (mSlowBallTimer > 0.0f) {
    mSlowBallTimer -= deltaTime;
    if (mSlowBallTimer <= 0.0f) {
      mSlowBallTimer = 0.0f;
      for (auto& ball : mBalls) {
        ball.SetSpeedMultiplier(2.0f);
      }
    }
  }
}

void Game::Shutdown() {
  mScoreText.Shutdown();
  mLivesText.Shutdown();
  mHighScoreText.Shutdown();

  mMenu.Shutdown();
  mOptionsMenu.Shutdown();

  mStatusText.Shutdown();

  if (mRenderer) {
    SDL_DestroyRenderer(mRenderer);
    mRenderer = nullptr;
  }

  if (mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
  }

  mAudio.Shutdown();

  TTF_Quit();
  SDL_Quit();
}

Game::~Game() { Shutdown(); }

std::string Game::GetAssetPath(const char *relativePath) const {
  const char *basePath = SDL_GetBasePath();

  if (!basePath) {
    return relativePath;
  }

  return std::string(basePath) + relativePath;
}
