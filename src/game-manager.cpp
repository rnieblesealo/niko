#include "game-manager.h"

GAME_MANAGER::GAME_MANAGER()
    : rng(static_cast<uint32_t>(
          std::chrono::system_clock::now().time_since_epoch().count()))
    , obstacle_spawn_distribution(0, 5)
    , coin_flip(HEADS_CHANCE) {};

void GAME_MANAGER::update()
{
  // NOTE: RESUME HERE!
}
