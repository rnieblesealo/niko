#include "raylib.h"
#include <cstdint>
#include <filesystem>

namespace {
const uint32_t SCREEN_WIDTH       = 400;
const uint32_t SCREEN_HEIGHT      = 400;
const uint32_t TARGET_FPS         = 60;
const int32_t SPRITE_SCALE_FACTOR = 4;
const Vector2 DIRECTIONS_4[4]{
    {0, -1},
    {0, 1},
    {-1, 0},
    {1, 0}}; // Used in direction-related iterative calculations

const float JUMP_MIN_HEIGHT       = 50;
const float JUMP_INITIAL_VELOCITY = 8;
const float JUMP_DROP_VELOCITY    = 2;
const float GRAVITY = -0.5; // Acceleration applied to jump velocity

const uint32_t FLOOR_Y_POS = 320;

} // namespace

namespace J4NK {
const Color NK_BLUE = {0x17, 0x73, 0xb8};
}

using namespace J4NK;

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

  uint32_t current_frame = 0;
  uint32_t frame_counter = 0;
  Vector2 position       = {100, 100};

  /**
   * @note  Only height gets divided by frame count to "cut up"
   * the individual frames (they are vertically stacked)
   */
  Rectangle frame_rect = {0, 0, static_cast<float>(spritesheet.width),
                          static_cast<float>(spritesheet.height) / frame_count};

  /* === Drawing === */

  float scaled_width  = SPRITE_SCALE_FACTOR * frame_rect.width;
  float scaled_height = SPRITE_SCALE_FACTOR * frame_rect.height;

  Rectangle draw_rect   = {0, 0, 0, 0};
  Vector2 origin_offset = {scaled_width / 2, scaled_height / 2};

  /* === Moving === */

  Vector2 velocity = {0, 0};

  /* === Jumping === */

  bool is_grounded    = false;
  float jump_velocity = 0;
  uint32_t jump_timer = 0;

  /* === Helper Functions === */

  void renderOutline() {
    /**
     * @brief Thickness of the outline
     *
     * We draw 4 black-tinted copies of the sprite offset by the same amount to top, bottom, left and right
     *
     * This creates a stroke effect! :)
     */
    const int outline_thickness_width = 4;
    for (const auto &dir : DIRECTIONS_4) {
      Rectangle outline = {position.x + outline_thickness_width * dir.x,
                           position.y + outline_thickness_width * dir.y,
                           scaled_width, scaled_height};

      DrawTexturePro(spritesheet, frame_rect, outline, origin_offset, 0, BLACK);
    }
  }

  void startJump() {
    this->jump_velocity = -JUMP_INITIAL_VELOCITY;
    this->is_grounded   = false;
  }

  void updateJump() {
    float feet_pos = this->position.y + 35;

    if (feet_pos > FLOOR_Y_POS) {
      this->is_grounded   = true;
      this->jump_velocity = 0;
      this->position.y    = FLOOR_Y_POS - 35;
    }

    else if (!is_grounded) {
      this->jump_velocity -= GRAVITY;
      this->position.y += jump_velocity;
    }

    /*
    // Reach max jump height
    if (this->position.y < JUMP_MAX_HEIGHT) {
      this->endJump();
    }

    // At ground level
    if (this->position.y > FLOOR_Y_POS) {
      this->reset();
    }
    */
  };

public:
  Niko() = default;

  ~Niko() { UnloadTexture(spritesheet); };

  bool debug = false;

  void advanceFrames() {
    frame_counter++;

    if (frame_counter >= (TARGET_FPS / frame_speed)) {
      frame_counter = 0;

      current_frame++;

      if (current_frame == frame_count) {
        current_frame = 0;
      }

      frame_rect.y = static_cast<float>(
          current_frame * static_cast<float>(spritesheet.height) / frame_count);
    }
  }

  void move() {
    // ---- CHROME DINO JUMP
    if (IsKeyPressed(KEY_SPACE)) {
      if (this->is_grounded) {
        startJump();
      }
    }

    updateJump();
  }

  void render() {
    renderOutline();

    Rectangle draw_rect = {position.x, position.y, scaled_width, scaled_height};

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

    DrawTexturePro(spritesheet, frame_rect, draw_rect, origin_offset, 0,
                   WHITE); // Sprite

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
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Niko");

  SetTargetFPS(60);

  /* === Cool Font === */

  Font impact_font = LoadFont("assets/impact.ttf");

  const uint32_t impact_font_size    = 32;
  const uint32_t impact_font_spacing = 2;

  /* === Clouds === */

  const Texture2D cloud =
      LoadTexture(std::filesystem::path("assets/cloud.png").c_str());

  /**
   * @brief Screen coordinates at which to draw clouds
   * Completely eyeballed!
   */
  const Vector2 cloud_positions[] = {
      {130, 22},
      {305, 70},
      {-16, 127},
  };

  /* === Niko === */

  Niko niko;
  niko.debug = false;

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

    ClearBackground(NK_BLUE);

    // Draw the floor
    DrawLineEx(Vector2{0, FLOOR_Y_POS}, Vector2{SCREEN_WIDTH, FLOOR_Y_POS}, 4,
               GREEN);

    // Draw a bunch of fuckin clouds

    // Make a slightly transparent white to make the clouds fade off a bit
    const uint8_t faded_white_alpha = 120; // Out of 255
    Color faded_white               = WHITE;
    faded_white.a                   = faded_white_alpha;

    // Draw each cloud at its specified pos
    for (const auto &pos : cloud_positions) {
      DrawTextureEx(cloud, pos, 0, SPRITE_SCALE_FACTOR, faded_white);
    }

    // Draw the title
    const std::string title_text =
        "Niko The\n\tNicotine-Addicted\n\t\tPunk Salamander";

    Vector2 title_bounds = MeasureTextEx(impact_font, title_text.c_str(),
                                         impact_font_size, impact_font_spacing);

    const float title_y_offset_from_center =
        -90; // To move the title up, down from its default position at center of scren

    /**
     * @note This semantically sucks but it works
     *
     * (Yes, it's the screen dest rect where title text is drawn)
     */
    Rectangle title_dest_rect = {SCREEN_WIDTH / 2.0F - title_bounds.x / 2,
                                 SCREEN_HEIGHT / 2.0F - title_bounds.y / 2 +
                                     title_y_offset_from_center,
                                 title_bounds.x, title_bounds.y};

    // Draw outline around title
    const int outline_thickness_width = 2;
    for (const auto &dir : DIRECTIONS_4) {
      DrawTextEx(impact_font, title_text.c_str(),
                 Vector2{title_dest_rect.x + outline_thickness_width * dir.x,
                         title_dest_rect.y + outline_thickness_width * dir.y},
                 impact_font_size, impact_font_spacing, DARKGRAY);
    }

    DrawTextEx(impact_font, title_text.c_str(),
               Vector2{title_dest_rect.x, title_dest_rect.y}, impact_font_size,
               impact_font_spacing, WHITE);

    niko.render();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
