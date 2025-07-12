#include "raylib.h"
#include <cstdint>
#include <filesystem>

namespace {
constexpr uint32_t IPHONE_SE_SCREEEN_WIDTH = 750;
constexpr uint32_t IPHONE_SE_SCREEN_HEIGHT = 1334;
constexpr uint32_t TARGET_FPS              = 60;
constexpr int32_t SPRITE_SCALE_FACTOR      = 5;
} // namespace

class Niko {
private:
  /* === Sprite === */

  /**
   * @note  If there ends up being a lot of assets we might want to move them to a JSON
   * Also probably wanna pass this as a dep
   */
  const Texture2D spritesheet =
      LoadTexture(std::filesystem::path("assets/niko.png").c_str());
  const uint32_t frame_count = 8;
  const uint32_t frame_speed = 8; // Frames per second
  
  uint32_t current_frame     = 0;
  uint32_t frame_counter     = 0;
  Vector2 position           = {100, 230};

  /**
   * @note  Only height gets divided by frame count to "cut up"
   * the individual frames (they are vertically stacked)
   */
  Rectangle frame_rect = {0, 0, static_cast<float>(spritesheet.width),
                          static_cast<float>(spritesheet.height) / frame_count};

  /* === Drawing === */

  const float scaled_width  = SPRITE_SCALE_FACTOR * frame_rect.width;
  const float scaled_height = SPRITE_SCALE_FACTOR * frame_rect.height;

  Rectangle draw_rect   = {0, 0, 0, 0};
  Vector2 origin_offset = {scaled_width / 2, scaled_height / 2};

  /* === Jumping === */

  bool is_jumping     = false;
  bool is_grounded    = false;
  uint32_t jump_timer = 0;
  Rectangle feet      = {0, 0, 0, 0};

public:
  Niko() = default;

  ~Niko() { UnloadTexture(spritesheet); };

  bool debug = false;

  void advanceFrames() {
    frame_counter++;

    if (frame_counter >= (TARGET_FPS / frame_speed)) {
      frame_counter = 0;

      current_frame++;

      if (current_frame > frame_count) {
        current_frame = 0;
      }

      frame_rect.y = static_cast<float>(
          current_frame * static_cast<float>(spritesheet.height) / frame_count);
    }
  }

  void move() {
    /**
     * @note feet rect is initially identical to draw_rect
     *
     * We apply custom width and height and apply some offsets to make them fit the player's sprite better
     *
     * These values are entirely eyeballed!
     */

    const uint32_t feet_width  = 40;
    const uint32_t feet_height = 10; // How tall the feet rect is

    const int32_t feet_y_offset = -12;
    const int32_t feet_x_offset = -9;

    feet = {position.x - draw_rect.width / 2 + feet_width + feet_x_offset,
            position.y + draw_rect.height / 2 - feet_height + feet_y_offset,
            feet_width, feet_height};

    if (debug) {
      DrawRectangleLinesEx(feet, 2, YELLOW);
    }
  }

  void render() {
    float scaled_width =
        SPRITE_SCALE_FACTOR * static_cast<float>(frame_rect.width);
    float scaled_height =
        SPRITE_SCALE_FACTOR * static_cast<float>(frame_rect.height);

    draw_rect = {position.x, position.y, scaled_width, scaled_height};

    /**
     * @note Raylib's only draw fuction that allows scaling AND using a src rect is DrawTexturePro, which is why we need to define raw rect and origin
     *
     * @field draw_rect is where on the screen we'll draw to
     * @field origin_offset is applied to the top left corner of
     * draw_rect; this offset also applies to our draw position, so to draw a
     * correct draw_rect, we must "undo" the offset
     *
     * I could probably explain this a bit better later on...
     */

    DrawTexturePro(spritesheet, frame_rect, draw_rect, origin_offset, 0, WHITE);

    if (debug) {
      DrawText("Niko the Nicotine-Addicted\nPunk Salamander",
               position.x - scaled_width / 2,
               position.y - scaled_width / 2 - 40, 18, WHITE);

      DrawText("POS", position.x - scaled_width / 2,
               position.y + scaled_height / 2, 12, BLUE);
      DrawCircleV(position, 2, BLUE);

      DrawText("ROT CTR", position.x - scaled_width / 2,
               position.y + scaled_height / 2 + 10, 12, GREEN);
      DrawCircleLinesV(Vector2{draw_rect.x, draw_rect.y}, 4, GREEN);

      DrawText("BOUNDS", position.x - scaled_width / 2,
               position.y + scaled_height / 2 + 20, 12, RED);
      DrawRectangleLinesEx(Rectangle{draw_rect.x - scaled_width / 2,
                                     draw_rect.y - scaled_width / 2,
                                     draw_rect.width, draw_rect.height},
                           1, RED); // debug
    }
  }
};

int main(void) {
  InitWindow(IPHONE_SE_SCREEEN_WIDTH / 2, IPHONE_SE_SCREEN_HEIGHT / 2, "Game");

  SetTargetFPS(60);

  Niko niko;
  niko.debug = true;

  while (!WindowShouldClose()) {
    /**
     * @note Update logic goes before render! When rendering, we want the most
     * up-to-date game; that makes sense
     */

    // ===================================
    // Game Logic
    // ===================================

    niko.move();
    niko.advanceFrames();

    // ===================================
    // Game Rendering
    // ===================================

    BeginDrawing();

    ClearBackground(BLACK);

    niko.render();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
