#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <random>

class GAME_MANAGER final
{
private:
  /**
   * @brief Chance that the below T/F distr. will land on True
   */
  const float HEADS_CHANCE = 0.75;

  /**
   * @brief Random number generator
   */
  std::mt19937 rng;

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

public:
  explicit GAME_MANAGER();

  void update();
};

#endif
