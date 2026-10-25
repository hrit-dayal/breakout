#include "high_score.hpp"

#include <SDL3/SDL.h>

#include <cstdio>

bool HighScore::Load()
{
    const std::string savePath = GetSavePath();

    if (savePath.empty())
        return false;

    FILE* file = std::fopen(savePath.c_str(), "r");

    if (!file)
    {
        mScore = 0;
        return false;
    }

    int score = 0;
    const bool success = std::fscanf(file, "%d", &score) == 1;

    std::fclose(file);

    if (!success || score < 0)
    {
        mScore = 0;
        return false;
    }

    mScore = score;
    return true;
}

bool HighScore::Save() const
{
    const std::string savePath = GetSavePath();

    if (savePath.empty())
        return false;

    FILE* file = std::fopen(savePath.c_str(), "w");

    if (!file)
        return false;

    const bool success = std::fprintf(file, "%d\n", mScore) > 0;

    std::fclose(file);
    return success;
}

void HighScore::SubmitScore(int score)
{
    if (score > mScore)
    {
        mScore = score;
        Save();
    }
}

int HighScore::GetScore() const
{
    return mScore;
}

std::string HighScore::GetSavePath() const
{
    std::string path;

    // SDL_GetPrefPath allocates the string and requires SDL to be
    // initialized, so this is only ever called after Game::Initialize().
    const char* prefPath = SDL_GetPrefPath("hrit-dayal", "Breakout");

    if (prefPath)
    {
        path = prefPath;
        path += "highscore.dat";

        SDL_free((void*)prefPath);
    }

    return path;
}
