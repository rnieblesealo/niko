#include "game-manager.h"
#include "constants.h"
#include <random>

GAME_MANAGER::GAME_MANAGER()
    : rng(static_cast<uint32_t>(
          // Seed RNG engine with time
          std::chrono::system_clock::now().time_since_epoch().count())) {};

void GAME_MANAGER::runObstacleSpawner(
    std::vector<std::shared_ptr<Texture2D>> const &obstacle_texture_pool)
{
  obstacle_spawn_timer++;

  if (obstacle_spawn_timer >= (TARGET_FPS / obstacle_spawn_rate))
  {
    // "Flip coin" (i.e. instantiate a quick bernoulli distr.) to figure out whether we will spawn
    bool will_spawn_obstacle = std::bernoulli_distribution{this->HEADS_CHANCE}(rng);
    if (will_spawn_obstacle)
    {
      // Select a random texture index (again instantiate int distr. and sample it)
      size_t chosen_texture_index = std::uniform_int_distribution<size_t>{
          0, obstacle_texture_pool.size() - 1}(rng); // Range passed is inclusive

      // Create an obstacle
      std::shared_ptr<Texture2D> const &obstacle_texture =
          obstacle_texture_pool.at(chosen_texture_index);
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
  // Move by gamespeed
  for (auto &obstacle : active_obstacles)
  {
    Rectangle &obstacle_dest_rect = obstacle.rect;
    obstacle_dest_rect.x -= BASE_GAME_SPEED;
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
            const Rectangle &draw_dest = obstacle.rect;
            return draw_dest.x < -draw_dest.width;
          }),
      active_obstacles.end());
}

bool GAME_MANAGER::nikoTouchingObstacle(NIKO &niko)
{
  auto player_collider = niko.getCollisionCircle();
  auto first_collided_obstacle =
      std::find_if(active_obstacles.begin(),
                   active_obstacles.end(),
                   [&](const auto &obstacle) -> bool
                   {
                     return CheckCollisionCircleRec(
                         player_collider.point, player_collider.radius, obstacle.rect);
                   });

  // Did a collision happen?
  return first_collided_obstacle != active_obstacles.end();
}

void GAME_MANAGER::renderObstacles()
{
  // Draw all active obstacles
  for (const auto &obstacle : active_obstacles)
  {
    DrawTextureEx(*obstacle.texture,
                  {obstacle.rect.x, obstacle.rect.y},
                  0,
                  static_cast<float>(OBSTACLE_DIMENSIONS.x) / obstacle.texture->width,
                  WHITE);
  }
}
