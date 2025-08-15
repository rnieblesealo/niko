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

  if (obstacle_spawn_timer >= (TARGET_FPS / OBSTACLE_SPAWN_RATE))
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

std::vector<Obstacle> const &GAME_MANAGER::getActiveObstacles()
{
  return this->active_obstacles;
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

void GAME_MANAGER::advanceState()
{
  // Before we directly set the state, check what numeric value next state would have
  // If it's offbounds, set it back to 0
  // Then set the enum
  // This helps address C++'s disallowing of doing arithmetic on enums and handles wrapping to the first state

  uint8_t next_state = static_cast<uint8_t>(current_state) + 1;
  if (next_state > static_cast<uint8_t>(GAME_STATE::GAME_OVER))
  {
    next_state = 0;
  }

  this->current_state = static_cast<GAME_STATE>(next_state);

  // Propagate state change to observers
  for (auto &observer : this->observers)
  {
    observer.get().onStateChangedTo(this->current_state);
  }
}

GAME_STATE GAME_MANAGER::getCurrentState() { return this->current_state; }

void GAME_MANAGER::addObserver(GAME_OBJECT_INTF &observer)
{
  this->observers.push_back(observer);
}

void GAME_MANAGER::removeObserver(GAME_OBJECT_INTF const &observer)
{
  this->observers.erase(std::remove_if(this->observers.begin(),
                                       this->observers.end(),
                                       [&](GAME_OBJECT_INTF const &curr) -> bool
                                       {
                                         // Custom types in C++ need to define equality operator or otherwise we must use their addresses
                                         // Since game object is a contract and it has no members that establish identity, we use mem. addresses
                                         return &curr == &observer;
                                       })

  );
}
