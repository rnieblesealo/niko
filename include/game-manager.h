#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "niko.h"
#include <algorithm>
#include <random>
#include <raylib.h>

class GAME_MANAGER final
{
private:
  /**
   * @brief Chance that the below T/F distr. will land on True
   */
  const float HEADS_CHANCE = 0.75;

  /**
   * @brief Dimensions of an obstacle
   */
  const Vector2 OBSTACLE_DIMENSIONS{60, 60};

  /**
   * @brief Interval of obstacle spawn
   * (In spawns per second)
   */
  const uint32_t obstacle_spawn_rate = 1;

  /**
   * @brief Timer to next obstacle spawn
   */
  uint32_t obstacle_spawn_timer = 0;

  /**
   * @brief Random number generator
   */
  std::mt19937 rng;

  /**
   * @brief Obstacle textures
   */
  std::vector<Texture2D> const &my_obstacle_textures;

  /**
   * @brief Distribution of obstacle texture indices
   * Sampled randomly to choose a texture for the next obstacle
   */
  std::uniform_int_distribution<std::size_t> obstacle_spawn_distribution;

  /**
   * @brief True/False distribution
   * Sampled randomly to decide whether to spawn an enemy or not this interval
   */
  std::bernoulli_distribution coin_flip;

  /**
   * @brief Onscreen obstacles
   * .first = Index of obstacle texture
   * .second = Dest rect of obstacle
   */
  std::vector<std::pair<uint32_t, Rectangle>> active_obstacles{};

  /**
   * @brief Advances obstacle spawn timer and spawns new obstacles according to set interval
   */
  void runObstacleSpawner();

  /**
   * @brief Moves active obstacles
   */
  void moveActiveObstacles();

  /**
   * @brief Deletes offscreen obstacles
   */
  void removeOffscreenObstacles();

  /**
   * @brief Checks if player is touching an obstacle
   */
  bool playerTouchingObstacle(const NIKO &niko)
  {
    active_obstacles.erase(std::find_if(
        active_obstacles.begin(),
        active_obstacles.end(),
        // Conditional deletion function
        // Any obstacle fully past the LEFT SIDE of screen ( x = 0 - obs.width ) is considered off bounds
        [&](const auto &obstacle) -> bool
        {
          // const Rectangle &obstacle_rect = obstacle.second;
          return true;
        }));

    return false;
  }

public:
  explicit GAME_MANAGER(std::vector<Texture2D> const &obstacle_textures);

  void renderObstacles();
  void updateObstacles();
};

#endif
