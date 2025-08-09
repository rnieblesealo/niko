#include "niko.h"
#include "constants.h"
#include "raylib.h"
#include "types.h"

void NIKO::startJump()
{
  if (!this->is_grounded)
  {
    return;
  }

  this->my_velocity.y = -JUMP_FORCE;
  this->is_grounded   = false;

  PlaySound(sfx_jump);
  my_spritesheet_renderer->setSpritesheet("jump");
}

void NIKO::setPosition(int32_t x, int32_t y)
{
  this->my_position.x = x;
  this->my_position.y = y;
}

const CollisionCircle NIKO::getCollisionCircle()
{
  Vector2  point  = Vector2{this->my_position.x, this->my_position.y};
  uint32_t radius = this->my_collision_radius;

  return CollisionCircle{point, radius};
}

void NIKO::endJump()
{
  this->my_velocity.y = 0;
  this->my_position.y = FLOOR_Y_POS - 35;
  this->is_grounded   = true;

  PlaySound(sfx_land);
  my_spritesheet_renderer->setSpritesheet("run");
}

void NIKO::update()
{
  my_position.y += this->my_velocity.y;
  float feet_pos = this->my_position.y + 35;

  if (!is_grounded)
  {
    this->my_velocity.y -= GRAVITY;

    if (feet_pos + 2.0f > FLOOR_Y_POS)
    {
      this->my_velocity.y = 2;
    }

    if (feet_pos >= FLOOR_Y_POS)
    {
      endJump();
    }
  }
}

void NIKO::render(bool debug_mode)
{
  const float NIKO_SPRITE_SCALE = 4;

  const float my_frame_width =
      NIKO_SPRITE_SCALE * this->my_spritesheet_renderer->getFrameWidth();
  const float my_frame_height =
      NIKO_SPRITE_SCALE * this->my_spritesheet_renderer->getFrameHeight();

  // We consider Niko's position to be at the center of his sprite
  // This draw dest position calculation accounts for that
  Rectangle my_draw_dest = {this->my_position.x - my_frame_width / 2,
                            this->my_position.y - my_frame_height / 2,
                            my_frame_width,
                            my_frame_height};

  this->my_spritesheet_renderer->render(my_draw_dest);

  if (debug_mode)
  {
    // Draw collider
    auto my_col_circ = this->getCollisionCircle();
    DrawCircleLines(
        my_col_circ.point.x, my_col_circ.point.y, my_col_circ.radius, MAGENTA);

    // Draw a point at Niko's position to visualize it!
    DrawCircleV(this->my_position, 2, GREEN);
  }
}
