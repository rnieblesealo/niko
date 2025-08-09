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

#endif
