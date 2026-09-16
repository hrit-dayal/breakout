#pragma once

#include <SDL3/SDL.h>

#include "text.hpp"

class OptionsMenu
{
public:
    OptionsMenu() = default;
    ~OptionsMenu();

    bool Initialize(
        SDL_Renderer* renderer,
        const char* fontPath
    );

    void Shutdown();

    void HandleKey(SDL_Keycode key);

    void Render(SDL_Renderer* renderer) const;

    int GetSelection() const;

    bool IsSoundEnabled() const;
    bool IsMusicEnabled() const;

    void ResetSelection();

private:
    SDL_Renderer* mRenderer = nullptr;

    Text mTitle;
    Text mSound;
    Text mMusic;
    Text mBack;

    int mSelection = 0;

    bool mSoundEnabled = true;
    bool mMusicEnabled = true;
};
