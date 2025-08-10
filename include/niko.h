#ifndef NIKO_H
#define NIKO_H

#include "game-object-intf.h"
#include "spritesheet-renderer.h"
#include "types.h"
#include <cstdint>
#include <filesystem>
#include <raylib.h>

class NIKO final : GAME_OBJECT_INTF
{
public:
  void update() override;
  void render(bool debug_mode = false) override;

private:
  const std::shared_ptr<SPRITESHEET_RENDERER> my_spritesheet_renderer;

  const Sound sfx_jump;
  const Sound sfx_land;

  Vector2  my_position;
  Vector2  my_velocity;
  float    my_collision_radius;
  uint32_t my_jump_timer;
  bool     is_grounded;

  void endJump();

public:
  NIKO(std::shared_ptr<SPRITESHEET_RENDERER> spritesheet_renderer)
      : my_position({Vector2{0, 0}})
      , my_velocity({Vector2{0, 0}})
      , my_collision_radius(20)
      , my_jump_timer(0)
      , is_grounded(false)
      , sfx_jump(LoadSound(std::filesystem::path("assets/jump.wav").c_str()))
      , sfx_land(LoadSound(std::filesystem::path("assets/land.wav").c_str()))
      , my_spritesheet_renderer(spritesheet_renderer)
  {
  }

  /**
   * @brief Checks for jump input and starts jump if input register
   */
  void startJump();

  /**
   * @brief Sets Niko's position
   * @param x Sets Niko's X coord
   * @param y Sets Niko's X coord
   */
  void                  setPosition(int32_t x, int32_t y);
  const CollisionCircle getCollisionCircle();
};

#endif
