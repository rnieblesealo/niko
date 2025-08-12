#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "types.h"
#include <random>
#include <raylib.h>

class GAME_MANAGER final
{
public:
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

private:
  GAME_MANAGER(); // Private constructor (singleton)
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
  const uint32_t OBSTACLE_SPAWN_RATE = 1;

  /**
   * @brief The current state
   */
  GAME_STATE current_state = GAME_STATE::TITLE;

  /**
   * @brief Timer to next obstacle spawn
   */
  uint32_t obstacle_spawn_timer = 0;

  /**
   * @brief Random number generator
   */
  std::mt19937 rng;

  /**
   * @brief Onscreen obstacles (affected by render and update)
   */
  std::vector<Obstacle> active_obstacles{};

public:
  // Singleton
  static GAME_MANAGER &getInstance()
  {
    static GAME_MANAGER instance; // Static in C++ = instantiated only ONCE
    return instance;
  }

  GAME_MANAGER(GAME_MANAGER const &) =
      delete; // Delete copy (why would we ever copy a SINGLEton?)
  void operator=(GAME_MANAGER const &) =
      delete; // Delete equality (why would we ever need to compare a SINGLEton?)

  /**
   * @brief Gets immutable ref to active obstacles
   */
  std::vector<Obstacle> const &getActiveObstacles();

  /**
   * @brief Gets the current state
   */
  GAME_MANAGER::GAME_STATE getCurrentState();

  /**
   * @brief Advances to the next state; see GAME_STATE enum
   */
  void advanceState();

  /**
   * @brief Advances obstacle spawn timer and spawns new obstacles according to set interval
   * @param obstacle_texture_pool Container for possible obstacle textures we may use in spawning a new one
   */
  void runObstacleSpawner(
      std::vector<std::shared_ptr<Texture2D>> const &obstacle_texture_pool);

  /**
   * @brief Moves active obstacles
   */
  void moveActiveObstacles();

  /**
   * @brief Deletes offscreen obstacles
   */
  void removeOffscreenObstacles();

  /**
   * @brief Renders active obstacles
   */
  void renderObstacles();
};

#endif
