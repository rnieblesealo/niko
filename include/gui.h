#include <cstdint>
#include <raylib.h>
#include <string>

class GUI final
{
public:
  /**
   * @brief Draws big text at the center
   * @param str The text to draw
   * @param font The font to use
   * @param font_size The font's size
   * @param y_offset The offset from the center of the screen of the title rect's center, in pixels
   * @param font_spacing The spacing of the text's letters
   * @param draw_outline Draw a poor man black outline around the text?
   *
   * Default arguments are based on ideal game title render
   *
   * WARNING: Due to how it's implemented, if outline width is too large it'll look really weird! 4 is the MAX
   * TODO: Make y offset relative! Positioning GUI should always be rel...
   */
  static void drawBigText(const std::string &str,
                          const Font        &font,
                          uint32_t           font_size     = 32,
                          uint32_t           font_spacing  = 2,
                          float              y_offset      = -90,
                          Color              text_color    = WHITE,
                          bool               draw_outline  = true,
                          float              outline_width = 2

  );
};
