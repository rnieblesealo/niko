#include "scene.h"
#include "constants.h"
#include <algorithm>
#include <raylib.h>

SCENE::SCENE(const Texture2D &floor_texture)
    : my_floor_texture(floor_texture)
    , my_floors{// Initially onscreen floor
                0,
                // Initally offscreen floor
                static_cast<float>(my_floor_texture.width)}
{
}

void SCENE::addFloorPropTexture(Texture2D &prop_texture)
{
  this->my_floor_prop_textures.push_back(std::reference_wrapper<Texture2D>(prop_texture));
}

void SCENE::drawStaticProp(Texture2D const &static_prop_texture,
                           Vector2          pos,
                           uint32_t         scale,
                           float            opacity)
{
  // Adjust normal white draw color to match passed opacity
  // This is useful for things like clouds
  // Passed opacity is a float 0-1

  const uint8_t MAX_ALPHA = 255;
  const uint8_t alpha_from_opacity =
      std::clamp<float>(round(opacity * MAX_ALPHA), 0, MAX_ALPHA);

  Color white_with_modified_alpha = WHITE;
  white_with_modified_alpha.a     = alpha_from_opacity;

  DrawTextureEx(static_prop_texture, pos, 0, scale, white_with_modified_alpha);
}

void SCENE::update()
{
  // Move the 2 floors to create effect
  for (auto &floor_x_pos : my_floors)
  {
    floor_x_pos -= BASE_GAME_SPEED;
    if (floor_x_pos < -static_cast<float>(SCREEN_WIDTH))
    {
      floor_x_pos =
          SCREEN_WIDTH -
          (abs(floor_x_pos) -
           abs(static_cast<float>(
               SCREEN_WIDTH))); // Compensate for the amount of space we went over the shift point
    }
  }
}

void SCENE::render(
    bool debug_mode = false) // WARNING: Resume here; I think this needs to be in the .h
{
  // Draw the floors
  for (const auto &floor_x_pos : my_floors)
  {
    DrawTexture(my_floor_texture, floor_x_pos, FLOOR_Y_POS, WHITE);
  }
}
