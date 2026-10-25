#pragma once

#include <string>

class HighScore
{
public:
    HighScore() = default;

    bool Load();
    bool Save() const;

    void SubmitScore(int score);

    int GetScore() const;

private:
    std::string GetSavePath() const;

    int mScore = 0;
};
