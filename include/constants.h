#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"
#include <cstdint>
#include <string>

namespace
{
/**
 * @brief The window title
 */
const std::string WINDOW_TITLE = "Niko";

/**
 * @brief Window dimensions
 */
const uint32_t SCREEN_WIDTH  = 400;
const uint32_t SCREEN_HEIGHT = 400;

/**
 * @brief Window FPS limit
 */
const uint32_t TARGET_FPS = 30;

/**
 * @brief Identity directions
 */
const Vector2 DIRECTIONS[4]{{0, -1}, // Up
                            {0, 1},  // Down
                            {-1, 0}, // Left
                            {1, 0}}; // Right

/**
 * @brief Impulse applied to player's jump
 */
const float JUMP_FORCE = 10;

/**
 * @brief Acceleration applied to player
 */
const float GRAVITY = -0.5;

/**
 * @brief Y-level of the floor
 */
const uint32_t FLOOR_Y_POS = 320;
} // namespace

/**
 * @brief Colors from 12J4NK color palette
 * @source https://lospec.com/palette-list/12j4nk
 */
namespace COLORPAL_12J4NK
{
const Color NK_BLUE = {23, 115, 184, 255}; // r, g, b, a
}

#endif
