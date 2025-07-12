#include "raylib.h"
#include <cstdint>
#include <filesystem>

namespace {
constexpr uint32_t IPHONE_SE_SCREEEN_WIDTH = 750;
constexpr uint32_t IPHONE_SE_SCREEN_HEIGHT = 1334;
constexpr uint32_t TARGET_FPS              = 60;
constexpr int32_t SPRITE_SCALE_FACTOR      = 6;
} // namespace

class Niko {
private:
  // If there ends up being a lot of assets we might want to move them to a JSON
  // Also, pass this as a dep if you want to get serious with it
  const Texture2D spritesheet =
      LoadTexture(std::filesystem::path("assets/niko.png").c_str());
  const uint32_t frame_count{8};
  const uint32_t frame_speed{8}; // Frames per second
  uint32_t current_frame{0};
  uint32_t frame_counter{0};
  Vector2 position{200, 200};
  Rectangle frame_rect{
      0, 0, static_cast<float>(spritesheet.width),
      static_cast<float>(spritesheet.height) /
          frame_count}; // Only height gets divided by frame count to "cut up"
                        // the individual frames (they are vertically stacked)

public:
  Niko() = default;

  ~Niko() { UnloadTexture(spritesheet); };

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

  void render(bool debug_overlay) {
    float scaled_width =
        SPRITE_SCALE_FACTOR * static_cast<float>(frame_rect.width);
    float scaled_height =
        SPRITE_SCALE_FACTOR * static_cast<float>(frame_rect.height);

    Rectangle draw_rect{position.x, position.y, scaled_width, scaled_height};
    Vector2 origin_offset{scaled_width / 2, scaled_height / 2};

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

    if (debug_overlay) {
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

  while (!WindowShouldClose()) {
    /**
     * @note Update logic goes before render! When rendering, we want the most
     * up-to-date game; that makes sense
     */

    // ===================================
    // Game Logic
    // ===================================

    niko.advanceFrames();

    // ===================================
    // Game Rendering
    // ===================================

    BeginDrawing();

    ClearBackground(BLACK);

    niko.render(true);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
