#include "game-manager.h"

GAME_MANAGER::GAME_MANAGER(std::vector<Texture2D> const &obstacle_textures)
    : my_obstacle_textures(obstacle_textures)
    , rng(static_cast<uint32_t>(
          // Seed RNG engine with time
          std::chrono::system_clock::now().time_since_epoch().count()))
    , obstacle_spawn_distribution(0, 5)
    , coin_flip(HEADS_CHANCE) {};

void GAME_MANAGER::update() {}
