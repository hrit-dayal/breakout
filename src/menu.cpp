#include "menu.hpp"

#include "constants.hpp"

Menu::~Menu()
{
    Shutdown();
}

bool Menu::Initialize(
    SDL_Renderer* renderer,
    const char* fontPath
)
{
    mRenderer = renderer;

    if (!mTitle.Initialize(
            renderer,
            fontPath,
            48.0f))
    {
        Shutdown();
        return false;
    }

    if (!mStart.Initialize(
            renderer,
            fontPath,
            28.0f))
    {
        Shutdown();
        return false;
    }

    if (!mOptions.Initialize(
            renderer,
            fontPath,
            28.0f))
    {
        Shutdown();
        return false;
    }

    if (!mQuit.Initialize(
            renderer,
            fontPath,
            28.0f))
    {
        Shutdown();
        return false;
    }

    mTitle.SetText("BREAKOUT");
    mStart.SetText("START GAME");
    mOptions.SetText("OPTIONS");
    mQuit.SetText("QUIT");

    mSelection = 0;

    return true;
}

void Menu::Shutdown()
{
    mTitle.Shutdown();
    mStart.Shutdown();
    mOptions.Shutdown();
    mQuit.Shutdown();

    mRenderer = nullptr;
}

void Menu::HandleKey(SDL_Keycode key)
{
    if (key == SDLK_UP)
    {
        --mSelection;

        if (mSelection < 0)
        {
            mSelection = 2;
        }
    }

    if (key == SDLK_DOWN)
    {
        ++mSelection;

        if (mSelection > 2)
        {
            mSelection = 0;
        }
    }
}

int Menu::GetSelection() const
{
    return mSelection;
}

void Menu::ResetSelection()
{
    mSelection = 0;
}

void Menu::Render(SDL_Renderer* renderer) const
{
    float titleX =
        (Constants::WindowWidth - mTitle.GetWidth())
        / 2.0f;

    float startX =
        (Constants::WindowWidth - mStart.GetWidth())
        / 2.0f;

    float optionsX =
        (Constants::WindowWidth - mOptions.GetWidth())
        / 2.0f;

    float quitX =
        (Constants::WindowWidth - mQuit.GetWidth())
        / 2.0f;

    mTitle.Render(
        titleX,
        100.0f
    );

    mStart.Render(
        startX,
        240.0f
    );

    mOptions.Render(
        optionsX,
        300.0f
    );

    mQuit.Render(
        quitX,
        360.0f
    );

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );

    float selectorX = 0.0f;
    float selectorY = 0.0f;

    switch (mSelection)
    {
    case 0:
        selectorX = startX - 35.0f;
        selectorY = 238.0f;
        break;

    case 1:
        selectorX = optionsX - 35.0f;
        selectorY = 298.0f;
        break;

    case 2:
        selectorX = quitX - 35.0f;
        selectorY = 358.0f;
        break;
    }

    SDL_FRect selector{
        selectorX,
        selectorY,
        20.0f,
        28.0f
    };

    SDL_RenderFillRect(
        renderer,
        &selector
    );
}
