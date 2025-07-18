#ifndef NIKO_H
#define NIKO_H

#include "spritesheet-renderer.h"
#include <cstdint>
#include <filesystem>
#include <raylib.h>

class NIKO final
{
private:
  // NOTE: `const` suff should not be modified
  const std::shared_ptr<SPRITESHEET_RENDERER> my_spritesheet_renderer;

  const Sound sfx_jump;
  const Sound sfx_land;

  Vector2  my_position;
  Vector2  my_velocity;
  uint32_t my_jump_timer;
  bool     is_grounded;

  void endJump();

public:
  NIKO(std::shared_ptr<SPRITESHEET_RENDERER> spritesheet_renderer)
      : my_position({Vector2{0, 0}})
      , my_velocity({Vector2{0, 0}})
      , my_jump_timer(0)
      , is_grounded(false)
      , sfx_jump(LoadSound(std::filesystem::path("assets/jump.wav").c_str()))
      , sfx_land(LoadSound(std::filesystem::path("assets/land.wav").c_str()))
      , my_spritesheet_renderer(spritesheet_renderer)
  {
  }

  // Action
  void startJump();

  // Getter/Setter
  void setPosition(int32_t x, int32_t y);

  // Update
  void update();
  void render();
};

#endif
