#ifndef NIKO_H
#define NIKO_H

#include "game-object-intf.h"
#include "spritesheet-renderer.h"
#include "types.h"
#include <cstdint>
#include <raylib.h>

class NIKO final : GAME_OBJECT_INTF
{
public:
  void update() override;
  void render(bool debug_mode = false) override;
  void onStateChangedTo(GAME_STATE state) override;

private:
  const std::shared_ptr<SPRITESHEET_RENDERER> my_spritesheet_renderer;

  const Sound sfx_jump;
  const Sound sfx_land;

  Vector2  my_position;
  Vector2  my_velocity;
  float    my_collision_radius;
  uint32_t my_jump_timer;
  bool     is_grounded;

  /**
   * @brief Sets velocity, position, effects and state according to starting a jump
   */
  void startJump();

  /**
   * @brief Sets velocity, position, effects and state according to landing from a jump
   */
  void endJump();

public:
  /**
   * @param spritesheet_renderer The spritesheet renderer for Niko
   */
  explicit NIKO(std::shared_ptr<SPRITESHEET_RENDERER> spritesheet_renderer);
  ~NIKO() override;

  /**
   * @brief Sets Niko's position
   * @param x Sets Niko's X coord
   * @param y Sets Niko's X coord
   */
  void setPosition(int32_t x, int32_t y);

  /**
   * @brief Gets Niko's collision circle
   */
  const CollisionCircle getCollisionCircle();

  /**
   * @brief Checks if Niko is colliding against any obstacles in the given collection
   */
  bool isTouchingAny(std::vector<Obstacle> obstacles);
};

#endif
