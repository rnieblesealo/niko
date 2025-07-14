#ifndef SPRITESHEET_RENDERER_H
#define SPRITESHEET_RENDERER_H

#include "raylib.h"
#include <cstdint>

namespace
{
/**
 * @brief Default sprite animation speed
 */
const uint32_t DEFAULT_FPS = 8;
} // namespace

/**
 * @brief Manages and renders frames from a vertically stacked spritesheet.
 *
 * This component simplifies rendering animated sprites, handling frame
 * advancement and source rectangle calculation. It requires a spritesheet
 * where frames are stacked vertically. The `render` method functions
 * similarly to Raylib's `DrawTexturePro`, using a destination rectangle
 * to define the on-screen position and scale. This design keeps external
 * scaling straightforward and convenient.
 */
class SPRITESHEET_RENDERER final
{
private:
  /**
   * @brief Spritesheet texture containing all vertically-stacked frames
   */
  Texture2D &my_spritesheet;

  /**
   * @brief Bounds of the current frame relative to the spritesheet
   */
  Rectangle my_frame_rect;

  /**
   * @brief The animation speed in FPS
   */
  uint32_t my_fps;

  /**
   * @brief Amount of frames in the spritesheet
   */
  uint32_t my_frame_count;

  /**
   * @brief Counts elapsed game frames since last frame advance
   */
  uint32_t my_frame_counter;

  /**
   * @brief The index of the currently displayed frame
   */
  uint32_t my_current_frame;

  /**
   * @brief Specifies whether to render an outline around this sprite
   */
  bool should_render_outline;

  /**
   * @brief Advances the spritesheet's animation
   */
  void advanceFrames();

  /**
   * @brief Renders a poor-man's outline around the spritesheet
   *
   * A poor man's outline = drawing 4 black-tinted copies of this spritesheet
   * and rendering each one offset slightly to each of the 4 directions
   * */
  void renderOutline(Rectangle &dest);

public:
  explicit SPRITESHEET_RENDERER(Texture2D &spritesheet, uint32_t frame_count)
      : my_spritesheet(spritesheet)
      , my_frame_count(frame_count)
      , my_fps(DEFAULT_FPS)
      , my_frame_counter(0)
      , my_current_frame(0)
      , should_render_outline(false)
      , my_frame_rect(Rectangle{0,
                                0,
                                static_cast<float>(spritesheet.width),
                                static_cast<float>(static_cast<float>(spritesheet.height) /
                                                   frame_count)}) // This assumes a vertically stacked spritesheet
  {
  }

  SPRITESHEET_RENDERER()                                        = delete;  // No default
  SPRITESHEET_RENDERER(const SPRITESHEET_RENDERER &)            = delete;  // No copy
  SPRITESHEET_RENDERER &operator=(const SPRITESHEET_RENDERER &) = delete;  // No copy assignment
  SPRITESHEET_RENDERER(SPRITESHEET_RENDERER &&)                 = delete;  // No move
  SPRITESHEET_RENDERER &operator=(SPRITESHEET_RENDERER &&)      = delete;  // No move assignment
  ~SPRITESHEET_RENDERER()                                       = default; // Default destruct

  /**
   * @brief Getters
   * @returns Unscaled width and height of a frame
   */
  float getFrameWidth();
  float getFrameHeight();

  /**
   * @brief Setters
   * @param t = the new value to use
   */
  void enableOutline(bool t);
  void setFPS(uint32_t t);

  /**
   * @brief Advances the animation and renders the animated spritesheet
   * @param dest Screen-relative rect where the sprite will be drawn
   */
  void render(Rectangle &dest);
};

#endif
