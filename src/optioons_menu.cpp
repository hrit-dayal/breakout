#include "options_menu.hpp"

#include "constants.hpp"

OptionsMenu::~OptionsMenu()
{
    Shutdown();
}

bool OptionsMenu::Initialize(
    SDL_Renderer* renderer,
    const char* fontPath
)
{
    mRenderer = renderer;

    if (!mTitle.Initialize(
            renderer,
            fontPath,
            42.0f))
    {
        Shutdown();
        return false;
    }

    if (!mSound.Initialize(
            renderer,
            fontPath,
            28.0f))
    {
        Shutdown();
        return false;
    }

    if (!mMusic.Initialize(
            renderer,
            fontPath,
            28.0f))
    {
        Shutdown();
        return false;
    }

    if (!mBack.Initialize(
            renderer,
            fontPath,
            28.0f))
    {
        Shutdown();
        return false;
    }

    mTitle.SetText("OPTIONS");
    mSound.SetText("SOUND: ON");
    mMusic.SetText("MUSIC: ON");
    mBack.SetText("BACK");

    mSelection = 0;
    mSoundEnabled = true;
    mMusicEnabled = true;

    return true;
}

void OptionsMenu::Shutdown()
{
    mTitle.Shutdown();
    mSound.Shutdown();
    mMusic.Shutdown();
    mBack.Shutdown();

    mRenderer = nullptr;
}

void OptionsMenu::HandleKey(SDL_Keycode key)
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

    if (key == SDLK_RETURN)
    {
        if (mSelection == 0)
        {
            mSoundEnabled = !mSoundEnabled;

            mSound.SetText(
                mSoundEnabled
                    ? "SOUND: ON"
                    : "SOUND: OFF"
            );
        }
        else if (mSelection == 1)
        {
            mMusicEnabled = !mMusicEnabled;

            mMusic.SetText(
                mMusicEnabled
                    ? "MUSIC: ON"
                    : "MUSIC: OFF"
            );
        }
    }
}

void OptionsMenu::Render(SDL_Renderer* renderer) const
{
    float titleX =
        (Constants::WindowWidth - mTitle.GetWidth())
        / 2.0f;

    float soundX =
        (Constants::WindowWidth - mSound.GetWidth())
        / 2.0f;

    float musicX =
        (Constants::WindowWidth - mMusic.GetWidth())
        / 2.0f;

    float backX =
        (Constants::WindowWidth - mBack.GetWidth())
        / 2.0f;

    mTitle.Render(
        titleX,
        100.0f
    );

    mSound.Render(
        soundX,
        240.0f
    );

    mMusic.Render(
        musicX,
        300.0f
    );

    mBack.Render(
        backX,
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
        selectorX = soundX - 35.0f;
        selectorY = 238.0f;
        break;

    case 1:
        selectorX = musicX - 35.0f;
        selectorY = 298.0f;
        break;

    case 2:
        selectorX = backX - 35.0f;
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

int OptionsMenu::GetSelection() const
{
    return mSelection;
}

bool OptionsMenu::IsSoundEnabled() const
{
    return mSoundEnabled;
}

bool OptionsMenu::IsMusicEnabled() const
{
    return mMusicEnabled;
}

void OptionsMenu::ResetSelection()
{
    mSelection = 0;
}
