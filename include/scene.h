#ifndef SCENE_H
#define SCENE_H

#include "game-object-intf.h"
#include <raylib.h>
#include <vector>

class SCENE final : GAME_OBJECT_INTF
{
public:
  void update() override;
  void render(bool debug_mode = false) override;

private:
  /**
   * @brief Texture of the floor
   */
  const Texture2D &my_floor_texture;

  /**
   * @brief Floor X positions
   * To create a "moving" floor we draw 2 floors at different X pos
   * All of them are always drawn at same Y pos
   *
   * WARNING: This assumes that all floors will have dimensions of given texture!
   */
  std::array<float, 2> my_floors;

  /**
   * @brief Stores textures of floor props
   * When spawning a floor prop, one of these will be randomly selected
   *
   * @note C++ disallows reference vectors since refs are non-assignable (vectors require assignable types)
   * We can circumvent this using std::reference_wrapper, which mimics an assignable reference
   */
  std::vector<std::reference_wrapper<Texture2D>> my_floor_prop_textures;

  /**
   * @brief Stores instantiated floor props
   * .first = The floor prop's dest draw rect
   * .second = The texture to draw on the rect
   */
  std::vector<std::pair<Rectangle, Texture2D &>> my_active_floor_props;

public:
  explicit SCENE(const Texture2D &floor_texture);

  /**
   * @brief Adds a floor prop
   */
  void addFloorPropTexture(Texture2D &prop_texture);

  /**
   * @brief Draws a static prop
   */
  void drawStaticProp(Texture2D const &static_prop_texture,
                      Vector2          pos,
                      uint32_t         scale   = 1,
                      float            opacity = 1);
};

#endif
