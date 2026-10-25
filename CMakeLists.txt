cmake_minimum_required(VERSION 3.25)

project(breakout
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------
# Try to find a locally installed SDL3 first
# ------------------------------------------------------------

find_package(SDL3 CONFIG QUIET)

# ------------------------------------------------------------
# If SDL3 was not found, download it with FetchContent
# ------------------------------------------------------------

if(NOT SDL3_FOUND)

  message(STATUS "SDL3 not found locally.")
  message(STATUS "Downloading SDL3...")

  include(FetchContent)

  FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )

  FetchContent_MakeAvailable(SDL3)

else()

  message(STATUS "Using locally installed SDL3.")

endif()

# ------------------------------------------------------------
# Try to find a locally installed SDL3_ttf first
# ------------------------------------------------------------

find_package(SDL3_ttf CONFIG QUIET)

# ------------------------------------------------------------
# If SDL3_ttf was not found, download it
# ------------------------------------------------------------

if(NOT SDL3_ttf_FOUND)

  message(STATUS "SDL3_ttf not found locally.")
  message(STATUS "Downloading SDL3_ttf...")

  include(FetchContent)

  FetchContent_Declare(
        SDL3_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )

  FetchContent_MakeAvailable(SDL3_ttf)

else()

  message(STATUS "Using locally installed SDL3_ttf.")

endif()

# ------------------------------------------------------------
# Try to find a locally installed SDL3_ttf first
# ------------------------------------------------------------

find_package(SDL3_mixer CONFIG QUIET)

# ------------------------------------------------------------
# If SDL3_ttf was not found, download it
# ------------------------------------------------------------

if(NOT SDL3_mixer_FOUND)

  message(STATUS "SDL3_mixer not found locally.")
  message(STATUS "Downloading SDL3_mixer...")

  include(FetchContent)

  FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG main
)

  FetchContent_MakeAvailable(SDL3_mixer)
else()

  message(STATUS "Using locally installed SDL3_ttf.")

endif()

# ------------------------------------------------------------
# Executable
# ------------------------------------------------------------

add_executable(breakout WIN32
    src/main.cpp
    src/game.cpp
    src/paddle.cpp
    src/ball.cpp
    src/brick.cpp
    src/text.cpp
    src/menu.cpp
    src/options_menu.cpp
    src/high_score.cpp
    src/audio.cpp
    src/particle.cpp
    src/powerup.cpp
)

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------

target_include_directories(breakout
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ------------------------------------------------------------
# Link SDL3
# ------------------------------------------------------------

target_link_libraries(breakout
    PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
)

# --------------------------------------------------
# Copy assets beside executable
# --------------------------------------------------

add_custom_command(
    TARGET breakout
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:breakout>/assets"

    COMMENT "Copying assets..."
)
