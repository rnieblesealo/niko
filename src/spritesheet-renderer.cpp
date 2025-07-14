#include "spritesheet-renderer.h"
#include "constants.h"

void SPRITESHEET_RENDERER::advanceFrames()
{
  this->my_frame_counter++;

  if (this->my_frame_counter >= (TARGET_FPS / this->my_fps))
  {
    this->my_frame_counter = 0;

    this->my_current_frame++;

    if (this->my_current_frame == this->my_frame_count)
    {
      this->my_current_frame = 0;
    }

    this->my_frame_rect.y = static_cast<float>(this->my_current_frame *
                                               static_cast<float>(this->my_spritesheet.height) / this->my_frame_count);
  }
}

float SPRITESHEET_RENDERER::getFrameWidth() { return this->my_frame_rect.width; }

float SPRITESHEET_RENDERER::getFrameHeight() { return this->my_frame_rect.height; }

void SPRITESHEET_RENDERER::setFPS(uint32_t t) { this->my_fps = t; }

void SPRITESHEET_RENDERER::enableOutline(bool t) { this->should_render_outline = t; }

void SPRITESHEET_RENDERER::render(Rectangle &dest)
{
  this->advanceFrames();

  if (this->should_render_outline)
  {
    this->renderOutline(dest);
  }

  DrawTexturePro(this->my_spritesheet,
                 this->my_frame_rect,
                 dest,
                 Vector2{0, 0}, // No origin offset
                 0,             // No rotation
                 WHITE);        // Normal color
}

void SPRITESHEET_RENDERER::renderOutline(Rectangle &dest)
{
  /**
   * @brief Thickness of the outline
   *
   * We draw 4 black-tinted copies of the sprite offset by the same amount to top, bottom, left and right
   * This creates a poor man's stroke effect! :)
   */
  const int outline_thickness_width = 4;
  for (const auto &dir : DIRECTIONS)
  {
    Rectangle dest_outline = {
        dest.x + outline_thickness_width * dir.x, dest.y + outline_thickness_width * dir.y, dest.width, dest.height};

    DrawTexturePro(this->my_spritesheet,
                   this->my_frame_rect,
                   dest_outline,
                   Vector2{0, 0}, // No origin offset
                   0,             // No rotation
                   BLACK);        // Fully black, NOTE: Can make into option!
  }
}
