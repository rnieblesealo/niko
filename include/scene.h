#ifndef SCENE_H
#define SCENE_H

#include "game-object-intf.h"
#include <raylib.h>
#include <vector>

class SCENE final : public GAME_OBJECT_INTF
{
private:
  /**
   * @brief Texture of the floor
   */
  const Texture2D &my_floor_texture;

  /**
   * @brief Floor dest rects
   * To create a "moving" floor we use 2 floors that change pos offscreen
   */
  Rectangle my_floor_a;
  Rectangle my_floor_b;

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
  void drawStaticProp(Texture2D &static_prop_texture, Vector2 pos, uint32_t scale);

  void update() override;
  void draw() override;
};

#endif
