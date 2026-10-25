#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string>

#include "ball.hpp"
#include "brick.hpp"
#include "constants.hpp"
#include "paddle.hpp"
#include "particle.hpp"
#include "powerup.hpp"
#include "text.hpp"
#include "menu.hpp"
#include "options_menu.hpp"
#include "high_score.hpp"
#include "audio.hpp"

/**
 * @enum GameState
 * @brief Enumerates the different states the game can be in.
 */
enum class GameState {
    Menu,
    Options,
    Playing,
    Paused,
    GameOver,
    Victory
};

/**
 * @class Game
 * @brief Represents the game state and main loop.
 * Handles initialization, game loop, event processing, updates, and rendering.
 */
class Game {
public:
    Game() = default;
    ~Game();

    /**
     * @brief Initializes the game.
     * Initializes SDL, creates a window and renderer, initializes various game components, and loads high scores.
     * @return true if initialization is successful, false otherwise.
     */
    bool Initialize();

    /**
     * @brief Runs the main game loop.
     * Continuously processes events, updates the game state, and renders the game until the game is over or the window is closed.
     */
    void Run();

    /**
     * @brief Shuts down the game.
     * Cleans up SDL resources, shuts down game components, and saves high scores if necessary.
     */
    void Shutdown();

private:
    /**
     * @brief Processes SDL events.
     * Handles keyboard input to control the game, menu navigation, and other game-related events.
     */
    void ProcessEvents();

    /**
     * @brief Updates the game state.
     * Calls the appropriate update method based on the current game state.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void Update(float deltaTime);

    /**
     * @brief Updates the game state when playing.
     * Handles ball movement, collision detection, updates scores, and checks game-ending conditions.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void UpdatePlaying(float deltaTime);

    /**
     * @brief Renders the game.
     * Clears the renderer, renders all game elements (bricks, paddle, ball, score, lives, high score, status text), and presents the rendered frame.
     */
    void Render();

    /**
     * @brief Renders text related to the current game state.
     * Renders the appropriate text (e.g., menu options, game status) based on the current game state.
     */
    void RenderStateText();

    /**
     * @brief Creates the game bricks.
     * Initializes a grid of bricks based on predefined constants and adds them to the game.
     */
    void CreateBricks();

    /**
     * @brief Advances to the next level.
     * Increments the level, recreates bricks with a new pattern, and resets balls and paddle.
     */
    void NextLevel();

    /**
     * @brief Starts a new game.
     * Resets the game state, initializes the paddle, ball, and bricks, and sets the game state to Playing.
     */
    void StartGame();

    /**
     * @brief Ends the current round and transitions to the final state.
     * Submits the current score to the high score system, updates the high score display, and sets the game state to the final state (either Victory or GameOver).
     * @param finalState The final state to transition to (either GameState::Victory or GameState::GameOver).
     */
    void EndRound(GameState finalState);

    /**
     * @brief Checks if all bricks are destroyed.
     * @return true if all bricks are destroyed, false otherwise.
     */
    bool AreAllBricksDestroyed() const;

    /**
     * @brief Updates the high score display text.
     * Formats the current high score and updates the text object displaying it.
     */
    void UpdateHighScoreText();

    /**
     * @brief Constructs the full path to an asset.
     * Combines the base path with a relative asset path to form a full path.
     * @param relativePath The relative path of the asset.
     * @return The full path to the asset.
     */
    std::string GetAssetPath(const char* relativePath) const;

    /**
     * @brief Spawns a power-up at a brick's position.
     * Randomly determines if a power-up should drop based on PowerUpChance.
     * @param x The x position where the power-up should spawn.
     * @param y The y position where the power-up should spawn.
     */
    void SpawnPowerUp(float x, float y);

    /**
     * @brief Updates all active power-ups.
     * Moves power-ups downward and checks for expiration.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void UpdatePowerUps(float deltaTime);

    /**
     * @brief Applies a power-up effect to the game.
     * Activates the appropriate effect based on the power-up type.
     * @param powerUp The power-up to apply.
     */
    void ApplyPowerUp(const PowerUp& powerUp);

    /**
     * @brief Updates power-up timers and expires effects.
     * @param deltaTime The time elapsed since the last frame in seconds.
     */
    void UpdatePowerUpTimers(float deltaTime);

private:
    SDL_Window* mWindow = nullptr;  // SDL window
    SDL_Renderer* mRenderer = nullptr;  // SDL renderer

    Paddle mPaddle;  // Player's paddle
    std::vector<Ball> mBalls;  // Active balls in play
    Menu mMenu;  // Main menu
    OptionsMenu mOptionsMenu;  // Options menu
    HighScore mHighScore;  // High score system
    Audio mAudio;  // Audio manager

    std::vector<Brick> mBricks;  // Vector of bricks in the game
    std::vector<PowerUp> mPowerUps;  // Active power-ups falling
    ParticleSystem mParticleSystem;  // Particle effects system

    Text mScoreText;  // Text for displaying the current score
    Text mLivesText;  // Text for displaying the remaining lives
    Text mHighScoreText;  // Text for displaying the high score
    Text mStatusText;  // Text for displaying game status messages
    Text mLevelText;  // Text for displaying the current level

    int mLives = Constants::StartingLives;  // Number of lives the player has
    int mScore = 0;  // Current score of the player
    int mCurrentLevel = 1;  // Current level number
    int mMenuSelection = 0;  // Current selection in the main menu

    // Power-up effect state
    float mWidePaddleTimer = 0.0f;  // Timer remaining for wide paddle effect
    float mSlowBallTimer = 0.0f;  // Timer remaining for slow ball effect
    float mOriginalPaddleWidth = Constants::PaddleWidth;  // Original paddle width before power-ups

    // Screen shake
    float mScreenShakeTimer = 0.0f;  // Timer remaining for screen shake
    float mScreenShakeIntensity = 0.0f;  // Intensity of screen shake

    GameState mState = GameState::Menu;  // Current game state
    bool mRunning = false;  // Flag indicating whether the game is running
};
