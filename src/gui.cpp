#include "gui.h"
#include "constants.h"

void GUI::drawBigText(const std::string &title_str,
                      const Font        &font,
                      uint32_t           font_size,
                      uint32_t           font_spacing,
                      float              y_offset,
                      Color              text_color,
                      bool               draw_outline,
                      float              outline_width)
{

  // Dimensions of the title
  const Vector2 TITLE_BOUNDS =
      MeasureTextEx(font, title_str.c_str(), font_size, font_spacing);

  // Screen dest rect of title
  const Vector2 TITLE_POS = {SCREEN_WIDTH / 2.0F - TITLE_BOUNDS.x / 2,
                             SCREEN_HEIGHT / 2.0F - TITLE_BOUNDS.y / 2 + y_offset};

  if (draw_outline)
  {
    for (const auto &dir : DIRECTIONS)
    {
      DrawTextEx(font,
                 title_str.c_str(),
                 Vector2{TITLE_POS.x + outline_width * dir.x,
                         TITLE_POS.y + outline_width * dir.y},
                 font_size,
                 font_spacing,
                 BLACK);
    }
  }

  DrawTextEx(font,
             title_str.c_str(),
             Vector2{TITLE_POS.x, TITLE_POS.y},
             font_size,
             font_spacing,
             text_color);
};
