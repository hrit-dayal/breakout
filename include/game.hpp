#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <string>

#include "ball.hpp"
#include "brick.hpp"
#include "constants.hpp"
#include "paddle.hpp"
#include "text.hpp"
#include "menu.hpp"
#include "options_menu.hpp"

enum class GameState
{
    Menu,
    Options,
    Playing,
    Paused,
    GameOver,
    Victory
};

class Game
{
public:
    Game() = default;
    ~Game();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    void ProcessEvents();

    void Update(float deltaTime);
    void UpdatePlaying(float deltaTime);

    void Render();
    void RenderStateText();

    void CreateBricks();

    void StartGame();
    bool AreAllBricksDestroyed() const;

    std::string GetAssetPath(const char* relativePath) const;

private:
    SDL_Window* mWindow = nullptr;
    SDL_Renderer* mRenderer = nullptr;

    Paddle mPaddle;
    Ball mBall;
    Menu mMenu;
    OptionsMenu mOptionsMenu;

    std::vector<Brick> mBricks;

    Text mScoreText;
    Text mLivesText;
    Text mStatusText;

    int mLives = Constants::StartingLives;
    int mScore = 0;
    int mMenuSelection = 0;

    GameState mState = GameState::Menu;

    bool mRunning = false;
};
