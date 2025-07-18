#include "niko.h"
#include "constants.h"

void NIKO::startJump()
{
  if (!this->is_grounded)
  {
    return;
  }

  this->my_velocity.y = -JUMP_FORCE;
  this->is_grounded   = false;

  PlaySound(sfx_jump);
}

void NIKO::setPosition(int32_t x, int32_t y)
{
  this->my_position.x = x;
  this->my_position.y = y;
}

void NIKO::endJump()
{
  this->my_velocity.y = 0;
  this->my_position.y = FLOOR_Y_POS - 35;
  this->is_grounded   = true;

  PlaySound(sfx_land);
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

void NIKO::render()
{
  const float my_sprite_scale = 4;

  const float my_frame_width = my_sprite_scale * this->my_spritesheet_renderer->getFrameWidth();
  const float my_height      = my_sprite_scale * this->my_spritesheet_renderer->getFrameHeight();

  // We consider Niko's position to be at the center of his sprite
  // This draw dest position calculation accounts for that
  Rectangle my_draw_dest = {
      this->my_position.x - my_frame_width / 2, this->my_position.y - my_height / 2, my_frame_width, my_height};

  this->my_spritesheet_renderer->render(my_draw_dest);

  // Draw a point at Niko's position to visualize it!
  DrawCircleV(this->my_position, 5, PINK);
}
