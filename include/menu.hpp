#pragma once

#include <SDL3/SDL.h>

#include "text.hpp"

class Menu
{
public:
    Menu() = default;
    ~Menu();

    bool Initialize(
        SDL_Renderer* renderer,
        const char* fontPath
    );

    void Shutdown();

    void HandleKey(SDL_Keycode key);

    void Render(SDL_Renderer* renderer) const;

    int GetSelection() const;

    void ResetSelection();

private:
    SDL_Renderer* mRenderer = nullptr;

    Text mTitle;
    Text mStart;
    Text mOptions;
    Text mQuit;

    int mSelection = 0;
};
