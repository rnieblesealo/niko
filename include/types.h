#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <memory>

/**
 * @brief Defines an obstacle object
 */
typedef struct Obstacle
{
  std::shared_ptr<Texture2D>
            texture; // Texture shares ownership between this and wherever it comes from
  Rectangle rect;
} Obstacle;

/**
 * @brief Collision circle defined by point + radius
 */
typedef struct CollisionCircle
{
  Vector2  point;
  uint32_t radius;
} CollisionCircle;

/**
 * @brief Game state, determines global behavior
 */
enum class GAME_STATE : uint8_t
{
  /**
   * @brief Title shown, no spawns, no "gameplay" is running; waiting for input to start!
   */
  TITLE = 0,

  /**
   * @brief Meat and bones; the "main" game
   */
  IN_GAME = 1,

  /**
   * @brief Everything is frozen and game over text is shown
   * @note This should ALWAYS be the last value!
   */
  GAME_OVER = 2
};

#endif
