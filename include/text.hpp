#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Text
{
public:
    Text() = default;

    ~Text();

    bool Initialize(
        SDL_Renderer* renderer,
        const char* fontPath,
        float fontSize
    );

    void Shutdown();

    void SetText(const char* text);

    void Render(float x, float y) const;

    int GetWidth() const;
    int GetHeight() const;

private:
    SDL_Renderer* mRenderer = nullptr;
    TTF_Font* mFont = nullptr;
    SDL_Texture* mTexture = nullptr;

    int mWidth = 0;
    int mHeight = 0;
};
