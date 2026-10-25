#include "audio.hpp"

Audio::Audio()
    : mInitialized(false),
      mEnabled(true),
      mMusicEnabled(true),
      mMixer(nullptr),
      mMusicTrack(nullptr),
      mPaddleSound(nullptr),
      mBrickSound(nullptr),
      mWallSound(nullptr),
      mMenuSound(nullptr),
      mGameOverSound(nullptr),
      mVictorySound(nullptr),
      mLoseSound(nullptr),
      mMusic(nullptr)
{
}

Audio::~Audio()
{
    Shutdown();
}

bool Audio::Initialize(
    const char* paddlePath,
    const char* brickPath,
    const char* wallPath,
    const char* menuPath,
    const char* gameOverPath,
    const char* victoryPath,
    const char* losePath,
    const char* musicPath)
{
    if (mInitialized)
        return true;

    if (!MIX_Init())
    {
        SDL_Log("Failed to initialize SDL_mixer: %s", SDL_GetError());
        return false;
    }

    // Set this immediately so Shutdown() cleans up partial initialization.
    mInitialized = true;

    mMixer = MIX_CreateMixerDevice(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        nullptr);

    if (!mMixer)
    {
        SDL_Log("Failed to create audio mixer: %s", SDL_GetError());
        Shutdown();
        return false;
    }

    mPaddleSound = MIX_LoadAudio(mMixer, paddlePath, true);
    mBrickSound = MIX_LoadAudio(mMixer, brickPath, true);
    mWallSound = MIX_LoadAudio(mMixer, wallPath, true);
    mMenuSound = MIX_LoadAudio(mMixer, menuPath, true);
    mGameOverSound = MIX_LoadAudio(mMixer, gameOverPath, true);
    mVictorySound = MIX_LoadAudio(mMixer, victoryPath, true);
    mLoseSound = MIX_LoadAudio(mMixer, losePath, true);
    mMusic = MIX_LoadAudio(mMixer, musicPath, true);

    if (!mPaddleSound || !mBrickSound || !mWallSound ||
        !mMenuSound || !mGameOverSound || !mVictorySound ||
        !mLoseSound || !mMusic)
    {
        SDL_Log("Failed to load one or more audio files: %s",
                SDL_GetError());
        Shutdown();
        return false;
    }

    mMusicTrack = MIX_CreateTrack(mMixer);

    if (!mMusicTrack)
    {
        SDL_Log("Failed to create music track: %s", SDL_GetError());
        Shutdown();
        return false;
    }

    if (!MIX_SetTrackAudio(mMusicTrack, mMusic))
    {
        SDL_Log("Failed to assign music to track: %s", SDL_GetError());
        Shutdown();
        return false;
    }

    if (mMusicEnabled)
        StartMusic();

    return true;
}

void Audio::Shutdown()
{
    StopMusic();

    if (mMusicTrack)
    {
        MIX_DestroyTrack(mMusicTrack);
        mMusicTrack = nullptr;
    }

    if (mPaddleSound)
    {
        MIX_DestroyAudio(mPaddleSound);
        mPaddleSound = nullptr;
    }

    if (mBrickSound)
    {
        MIX_DestroyAudio(mBrickSound);
        mBrickSound = nullptr;
    }

    if (mWallSound)
    {
        MIX_DestroyAudio(mWallSound);
        mWallSound = nullptr;
    }

    if (mMenuSound)
    {
        MIX_DestroyAudio(mMenuSound);
        mMenuSound = nullptr;
    }

    if (mGameOverSound)
    {
        MIX_DestroyAudio(mGameOverSound);
        mGameOverSound = nullptr;
    }

    if (mVictorySound)
    {
        MIX_DestroyAudio(mVictorySound);
        mVictorySound = nullptr;
    }

    if (mLoseSound)
    {
        MIX_DestroyAudio(mLoseSound);
        mLoseSound = nullptr;
    }

    if (mMusic)
    {
        MIX_DestroyAudio(mMusic);
        mMusic = nullptr;
    }

    if (mMixer)
    {
        MIX_DestroyMixer(mMixer);
        mMixer = nullptr;
    }

    if (mInitialized)
    {
        MIX_Quit();
        mInitialized = false;
    }
}

void Audio::Play(MIX_Audio* sound)
{
    if (!mInitialized || !mEnabled || !sound || !mMixer)
        return;

    MIX_PlayAudio(mMixer, sound);
}

void Audio::PlayPaddle()
{
    Play(mPaddleSound);
}

void Audio::PlayBrick()
{
    Play(mBrickSound);
}

void Audio::PlayWall()
{
    Play(mWallSound);
}

void Audio::PlayMenu()
{
    Play(mMenuSound);
}

void Audio::PlayGameOver()
{
    Play(mGameOverSound);
}

void Audio::PlayVictory()
{
    Play(mVictorySound);
}

void Audio::PlayLose()
{
    Play(mLoseSound);
}

void Audio::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}

bool Audio::IsEnabled() const
{
    return mEnabled;
}

void Audio::StartMusic()
{
    if (!mInitialized || !mMusicEnabled || !mMusicTrack)
        return;

    SDL_PropertiesID options = SDL_CreateProperties();

    if (!options)
    {
        SDL_Log("Failed to create music options: %s", SDL_GetError());
        return;
    }

    SDL_SetNumberProperty(
        options,
        MIX_PROP_PLAY_LOOPS_NUMBER,
        -1);

    if (!MIX_PlayTrack(mMusicTrack, options))
    {
        SDL_Log("Failed to play background music: %s", SDL_GetError());
    }

    SDL_DestroyProperties(options);
}

void Audio::StopMusic()
{
    if (mMusicTrack)
        MIX_StopTrack(mMusicTrack, 0);
}

void Audio::SetMusicEnabled(bool enabled)
{
    if (mMusicEnabled == enabled)
        return;

    mMusicEnabled = enabled;

    if (mMusicEnabled)
        StartMusic();
    else
        StopMusic();
}

bool Audio::IsMusicEnabled() const
{
    return mMusicEnabled;
}
