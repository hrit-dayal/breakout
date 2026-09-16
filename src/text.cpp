#include "text.hpp"

Text::~Text()
{
    Shutdown();
}

bool Text::Initialize(
    SDL_Renderer* renderer,
    const char* fontPath,
    float fontSize
)
{
    mRenderer = renderer;

    mFont = TTF_OpenFont(
        fontPath,
        fontSize
    );

    if (!mFont)
    {
        SDL_Log(
            "TTF_OpenFont failed: %s",
            SDL_GetError()
        );

        return false;
    }

    return true;
}

void Text::Shutdown()
{
    if (mTexture)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }

    if (mFont)
    {
        TTF_CloseFont(mFont);
        mFont = nullptr;
    }

    mRenderer = nullptr;

    mWidth = 0;
    mHeight = 0;
}

void Text::SetText(const char* text)
{
    if (!mFont || !mRenderer)
    {
        return;
    }

    if (mTexture)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }

    SDL_Color color{
        255,
        255,
        255,
        255
    };

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            mFont,
            text,
            0,
            color
        );

    if (!surface)
    {
        SDL_Log(
            "TTF_RenderText_Blended failed: %s",
            SDL_GetError()
        );

        return;
    }

    mTexture =
        SDL_CreateTextureFromSurface(
            mRenderer,
            surface
        );

    if (!mTexture)
    {
        SDL_Log(
            "SDL_CreateTextureFromSurface failed: %s",
            SDL_GetError()
        );

        SDL_DestroySurface(surface);

        return;
    }

    mWidth = surface->w;
    mHeight = surface->h;

    SDL_DestroySurface(surface);
}

void Text::Render(float x, float y) const
{
    if (!mTexture)
    {
        return;
    }

    SDL_FRect destination{
        x,
        y,
        static_cast<float>(mWidth),
        static_cast<float>(mHeight)
    };

    SDL_RenderTexture(
        mRenderer,
        mTexture,
        nullptr,
        &destination
    );
}

int Text::GetWidth() const
{
    return mWidth;
}

int Text::GetHeight() const
{
    return mHeight;
}
