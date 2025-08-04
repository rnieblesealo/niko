#include "game-manager.h"
#include "constants.h"
#include <iostream>

GAME_MANAGER::GAME_MANAGER(std::vector<Texture2D> const &obstacle_textures)
    : my_obstacle_textures(obstacle_textures)
    , rng(static_cast<uint32_t>(
          // Seed RNG engine with time
          std::chrono::system_clock::now().time_since_epoch().count()))
    , obstacle_spawn_distribution(0, 5)
    , coin_flip(HEADS_CHANCE) {};

void GAME_MANAGER::runObstacleSpawner()
{
  obstacle_spawn_timer++;

  if (obstacle_spawn_timer >= (TARGET_FPS / obstacle_spawn_rate))
  {
    // Flip coin to see if we will spawn obstacle this interval
    bool will_spawn_obstacle = coin_flip(rng);
    if (will_spawn_obstacle)
    {
      // Select a texture and initialize the dest. rect of this obstacle
      int       obstacle_texture   = static_cast<int>(obstacle_spawn_distribution(rng));
      Rectangle obstacle_dest_rect = Rectangle{SCREEN_WIDTH + OBSTACLE_DIMENSIONS.x,
                                               FLOOR_Y_POS - OBSTACLE_DIMENSIONS.y,
                                               OBSTACLE_DIMENSIONS.x,
                                               OBSTACLE_DIMENSIONS.y};

      // Spawn it
      active_obstacles.push_back({obstacle_texture, obstacle_dest_rect});
    }

    // Reset the timer
    obstacle_spawn_timer = 0;
  }
}

void GAME_MANAGER::moveActiveObstacles()
{
  // Moved by the gamespeed
  for (auto &obstacle : active_obstacles)
  {
    Rectangle &obstacle_dest_rect = obstacle.second;

    obstacle_dest_rect.x -= GAME_SPEED;
  }
}

void GAME_MANAGER::removeOffscreenObstacles()
{
  active_obstacles.erase(
      std::remove_if(
          active_obstacles.begin(),
          active_obstacles.end(),
          // Conditional deletion function
          // Any obstacle fully past the LEFT SIDE of screen ( x = 0 - obs.width ) is considered off bounds
          [&](const auto &obstacle)
          {
            const Rectangle &draw_dest = obstacle.second;
            return draw_dest.x < -draw_dest.width;
          }),
      active_obstacles.end());
}

void GAME_MANAGER::updateObstacles()
{
  runObstacleSpawner();
  moveActiveObstacles();
  removeOffscreenObstacles();
}

void GAME_MANAGER::renderObstacles()
{
  // Draw all active obstacles
  for (const auto &obstacle : active_obstacles)
  {
    const int        texture_index = obstacle.first;
    const Rectangle &draw_dest     = obstacle.second;

    DrawTextureEx(my_obstacle_textures[texture_index],
                  {draw_dest.x, draw_dest.y},
                  0,
                  static_cast<float>(OBSTACLE_DIMENSIONS.x) /
                      my_obstacle_textures[texture_index].width,
                  WHITE);
  }
}
