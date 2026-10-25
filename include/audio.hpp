#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

class Audio
{
public:
    Audio();
    ~Audio();

    bool Initialize(
        const char* paddlePath,
        const char* brickPath,
        const char* wallPath,
        const char* menuPath,
        const char* gameOverPath,
        const char* victoryPath,
        const char* losePath,
        const char* musicPath
    );

    void Shutdown();

    void PlayPaddle();
    void PlayBrick();
    void PlayWall();
    void PlayMenu();
    void PlayGameOver();
    void PlayVictory();
    void PlayLose();

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    void SetMusicEnabled(bool enabled);
    bool IsMusicEnabled() const;

private:
    bool mInitialized;
    bool mEnabled;
    bool mMusicEnabled;

    MIX_Mixer* mMixer;
    MIX_Track* mMusicTrack;

    MIX_Audio* mPaddleSound;
    MIX_Audio* mBrickSound;
    MIX_Audio* mWallSound;
    MIX_Audio* mMenuSound;
    MIX_Audio* mGameOverSound;
    MIX_Audio* mVictorySound;
    MIX_Audio* mLoseSound;
    MIX_Audio* mMusic;

    void Play(MIX_Audio* sound);
    void StartMusic();
    void StopMusic();
};
