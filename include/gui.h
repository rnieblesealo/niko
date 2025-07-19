#include <cstdint>
#include <raylib.h>
#include <string>

class GUI final
{
public:
  /**
   * @brief Draws the game's title
   */
  static void drawTitle(const std::string &title_str,
                        const Font        &font,
                        uint32_t           font_size,
                        uint32_t           font_spacing,
                        float              y_offset,
                        bool               draw_outline);
};
