#include "gui.h"
#include "constants.h"

void GUI::drawTitle(const std::string &title_str,
                    const Font        &font,
                    uint32_t           font_size,
                    uint32_t           font_spacing,
                    float              y_offset,
                    bool               draw_outline = true)
{

  // Dimensions of the title
  const Vector2 title_bounds = MeasureTextEx(font, title_str.c_str(), font_size, font_spacing);

  // Screen dest rect of title
  const Vector2 title_pos = {SCREEN_WIDTH / 2.0F - title_bounds.x / 2,
                             SCREEN_HEIGHT / 2.0F - title_bounds.y / 2 + y_offset};

  if (draw_outline)
  {
    const int outline_stroke_width = 2;

    for (const auto &dir : DIRECTIONS)
    {
      DrawTextEx(font,
                 title_str.c_str(),
                 Vector2{title_pos.x + outline_stroke_width * dir.x, title_pos.y + outline_stroke_width * dir.y},
                 font_size,
                 font_spacing,
                 BLACK);
    }
  }

  DrawTextEx(font, title_str.c_str(), Vector2{title_pos.x, title_pos.y}, font_size, font_spacing, WHITE);
};
