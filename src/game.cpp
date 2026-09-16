#include "game.hpp"

#include <SDL3_ttf/SDL_ttf.h>

bool Game::Initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Breakout",
        Constants::WindowWidth,
        Constants::WindowHeight,
        SDL_WINDOW_RESIZABLE
    );

    if (!mWindow)
    {
        SDL_Log(
            "SDL_CreateWindow failed: %s",
            SDL_GetError()
        );

        TTF_Quit();
        SDL_Quit();

        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        nullptr
    );

    if (!mRenderer)
    {
        SDL_Log(
            "SDL_CreateRenderer failed: %s",
            SDL_GetError()
        );

        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;

        TTF_Quit();
        SDL_Quit();

        return false;
    }

    std::string fontPath =
        GetAssetPath("assets/fonts/ARIAL.TTF");

    if (!mScoreText.Initialize(
            mRenderer,
            fontPath.c_str(),
            24.0f))
    {
        Shutdown();
        return false;
    }

    if (!mLivesText.Initialize(
            mRenderer,
            fontPath.c_str(),
            24.0f))
    {
        Shutdown();
        return false;
    }

    if (!mMenu.Initialize(
            mRenderer,
            fontPath.c_str()))
    {
        Shutdown();
        return false;
    }

    if (!mOptionsMenu.Initialize(
            mRenderer,
            fontPath.c_str()))
    {
        Shutdown();
        return false;
    }

    if (!mStatusText.Initialize(
            mRenderer,
            fontPath.c_str(),
            32.0f))
    {
        Shutdown();
        return false;
    }

    mScoreText.SetText("Score: 0");
    mLivesText.SetText("Lives: 3");

    mMenuSelection = 0;

    mLives = Constants::StartingLives;
    mScore = 0;

    mState = GameState::Menu;
    mRunning = true;

    return true;
}

void Game::Run()
{
    Uint64 previousTime = SDL_GetTicks();

    while (mRunning)
    {
        Uint64 currentTime = SDL_GetTicks();

        float deltaTime =
            static_cast<float>(
                currentTime - previousTime
            ) / 1000.0f;

        previousTime = currentTime;

        if (deltaTime > 0.1f)
        {
            deltaTime = 0.1f;
        }

        ProcessEvents();
        Update(deltaTime);
        Render();
    }
}

void Game::ProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:

            mRunning = false;

            break;

        case SDL_EVENT_KEY_DOWN:

            if (event.key.key == SDLK_Q)
            {
                mRunning = false;
                break;
            }

            if (mState == GameState::Menu)
            {
                if (event.key.key == SDLK_UP ||
                        event.key.key == SDLK_DOWN)
                    {
                        mMenu.HandleKey(event.key.key);
                    }

                    if (event.key.key == SDLK_RETURN)
                    {
                        switch (mMenu.GetSelection())
                        {
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
                    }

                    if (event.key.key == SDLK_ESCAPE)
                    {
                        mRunning = false;
                    }

                    break;
            }

            if (mState == GameState::Options)
            {
                if (event.key.key == SDLK_UP ||
                    event.key.key == SDLK_DOWN)
                {
                    mOptionsMenu.HandleKey(
                        event.key.key
                    );
                }
                else if (event.key.key == SDLK_RETURN)
                {
                    if (mOptionsMenu.GetSelection() == 2)
                    {
                        mMenu.ResetSelection();
                        mState = GameState::Menu;
                    }
                    else
                    {
                        mOptionsMenu.HandleKey(
                            event.key.key
                        );
                    }
                }
                else if (event.key.key == SDLK_ESCAPE)
                {
                    mMenu.ResetSelection();
                    mState = GameState::Menu;
                }

                break;
            }

            if (event.key.key == SDLK_ESCAPE)
            {
                if (mState == GameState::Playing)
                {
                    mState = GameState::Paused;
                }
                else if (mState == GameState::Paused)
                {
                    mState = GameState::Playing;
                }
            }

            if (event.key.key == SDLK_RETURN)
            {
                if (mState == GameState::Paused)
                {
                    mState = GameState::Playing;
                }
                else if (mState == GameState::GameOver)
                {
                    StartGame();
                }
                else if (mState == GameState::Victory)
                {
                    StartGame();
                }
            }

            if (mState == GameState::Playing)
            {
                if (event.key.key == SDLK_LEFT)
                {
                    mPaddle.SetMovingLeft(true);
                }

                if (event.key.key == SDLK_RIGHT)
                {
                    mPaddle.SetMovingRight(true);
                }
            }

            break;

        case SDL_EVENT_KEY_UP:

            if (event.key.key == SDLK_LEFT)
            {
                mPaddle.SetMovingLeft(false);
            }

            if (event.key.key == SDLK_RIGHT)
            {
                mPaddle.SetMovingRight(false);
            }

            break;

        default:
            break;
        }
    }
}

void Game::Update(float deltaTime)
{
    switch (mState)
    {
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

void Game::UpdatePlaying(float deltaTime)
{
    mPaddle.Update(deltaTime);
    mBall.Update(deltaTime);

    SDL_FRect paddleRect =
        mPaddle.GetRect();

    if (mBall.CollidesWith(paddleRect))
    {
        mBall.BounceOffPaddle(paddleRect);
    }

    for (Brick& brick : mBricks)
    {
        if (brick.IsDestroyed())
        {
            continue;
        }

        if (mBall.CollidesWith(brick.GetRect()))
        {
            mBall.BounceOffBrick(
                brick.GetRect()
            );

            brick.Destroy();

            mScore += Constants::BrickScore;

            char scoreText[64];

            SDL_snprintf(
                scoreText,
                sizeof(scoreText),
                "Score: %d",
                mScore
            );

            mScoreText.SetText(scoreText);

            break;
        }
    }

    if (AreAllBricksDestroyed())
    {
        mState = GameState::Victory;
        return;
    }

    if (mBall.IsBelowScreen())
    {
        --mLives;

        char livesText[64];

        SDL_snprintf(
            livesText,
            sizeof(livesText),
            "Lives: %d",
            mLives
        );

        mLivesText.SetText(livesText);

        if (mLives > 0)
        {
            mBall.Reset();
        }
        else
        {
            mState = GameState::GameOver;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        10,
        10,
        20,
        255
    );

    SDL_RenderClear(mRenderer);

    if (mState == GameState::Playing ||
        mState == GameState::Paused)
    {
        for (const Brick& brick : mBricks)
        {
            brick.Render(mRenderer);
        }

        mPaddle.Render(mRenderer);
        mBall.Render(mRenderer);

        mScoreText.Render(20.0f, 15.0f);
        mLivesText.Render(680.0f, 15.0f);
    }

    RenderStateText();

    SDL_RenderPresent(mRenderer);
}

void Game::RenderStateText()
{
    switch (mState)
    {
    case GameState::Menu:
    {
        mMenu.Render(mRenderer);

        break;
    }

    case GameState::Options:

        mOptionsMenu.Render(mRenderer);

        break;

    case GameState::Paused:

        mStatusText.SetText(
            "PAUSED - Press ENTER to Continue"
        );

        mStatusText.Render(
            170.0f,
            280.0f
        );

        break;

    case GameState::GameOver:

        mStatusText.SetText(
            "GAME OVER - Press ENTER to Restart"
        );

        mStatusText.Render(
            140.0f,
            280.0f
        );

        break;

    case GameState::Victory:

        mStatusText.SetText(
            "YOU WIN - Press ENTER to Restart"
        );

        mStatusText.Render(
            160.0f,
            280.0f
        );

        break;

    case GameState::Playing:

        break;
    }
}

void Game::CreateBricks()
{
    mBricks.clear();

    const float totalWidth =
        Constants::BrickColumns * Constants::BrickWidth
        + (Constants::BrickColumns - 1)
            * Constants::BrickGap;

    const float startX =
        (Constants::WindowWidth - totalWidth)
        / 2.0f;

    for (int row = 0;
         row < Constants::BrickRows;
         ++row)
    {
        for (int column = 0;
             column < Constants::BrickColumns;
             ++column)
        {
            float x =
                startX
                + column
                    * (Constants::BrickWidth
                       + Constants::BrickGap);

            float y =
                Constants::BrickTopMargin
                + row
                    * (Constants::BrickHeight
                       + Constants::BrickGap);

            Brick brick;

            brick.Initialize(x, y);

            mBricks.push_back(brick);
        }
    }
}

void Game::StartGame()
{
    mLives = Constants::StartingLives;
    mScore = 0;

    mPaddle.Initialize();
    mBall.Initialize();

    CreateBricks();

    mScoreText.SetText("Score: 0");
    mLivesText.SetText("Lives: 3");

    mState = GameState::Playing;
}

bool Game::AreAllBricksDestroyed() const
{
    for (const Brick& brick : mBricks)
    {
        if (!brick.IsDestroyed())
        {
            return false;
        }
    }

    return true;
}

void Game::Shutdown()
{
    mScoreText.Shutdown();
    mLivesText.Shutdown();

    mMenu.Shutdown();
    mOptionsMenu.Shutdown();

    mStatusText.Shutdown();

    if (mRenderer)
    {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }

    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}

Game::~Game()
{
    Shutdown();
}

std::string Game::GetAssetPath(
    const char* relativePath
) const
{
    const char* basePath =
        SDL_GetBasePath();

    if (!basePath)
    {
        return relativePath;
    }

    return std::string(basePath)
        + relativePath;
}
