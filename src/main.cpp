#include "constants.h"
#include "gui.h"
#include "niko.h"
#include "spritesheet-renderer.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <random>
#include <raylib.h>

using namespace COLORPAL_12J4NK;

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Niko");
  InitAudioDevice();

  SetTargetFPS(60);

  /* === Cool Font === */

  Font IMPACT_FONT = LoadFont("assets/impact.ttf");

  const uint32_t IMPACT_FONT_size    = 32;
  const uint32_t IMPACT_FONT_spacing = 2;

  /* === Scene === */

  const Texture2D sand = LoadTexture(std::filesystem::path("assets/sand.png").c_str());

  float floor_chunk_x_positions[2] = {0, static_cast<float>(sand.width)};

  const Texture2D cloud = LoadTexture(std::filesystem::path("assets/cloud.png").c_str());

  /**
   * @brief Screen coordinates at which to draw clouds
   * Completely eyeballed!
   */
  const Vector2 cloud_positions[] = {
      {130, 22},
      {305, 70},
      {-16, 127},
  };

  // Load spritesheet textures
  Texture2D spr_niko_run  = LoadTexture(std::filesystem::path("assets/niko-run.png").c_str());
  Texture2D spr_niko_jump = LoadTexture(std::filesystem::path("assets/niko-jump.png").c_str());

  // Organize them
  std::map<std::string, SPRITESHEET const &> const &niko_spritesheets{{"run", SPRITESHEET{spr_niko_run, 8}},
                                                                      {"jump", SPRITESHEET{spr_niko_jump, 1}}};

  // Create renderer with them
  std::shared_ptr<SPRITESHEET_RENDERER> niko_spritesheet_renderer(new SPRITESHEET_RENDERER(niko_spritesheets));

  // Apply our settings
  niko_spritesheet_renderer->setFPS(4);
  niko_spritesheet_renderer->enableOutline(true);
  niko_spritesheet_renderer->setSpritesheet("run");

  // Instantiate niko himself
  NIKO niko(niko_spritesheet_renderer);

  niko.setPosition(100, FLOOR_Y_POS);

  /* === Obstacles === */

  const std::array<Texture2D, 3> obs_short = {
      LoadTexture(std::filesystem::path("assets/tate.png").c_str()),
      LoadTexture(std::filesystem::path("assets/trump.png").c_str()),
      LoadTexture(std::filesystem::path("assets/netanyahu.png").c_str()),
  };

  std::vector<std::pair<uint32_t, Rectangle>> obstacles{};

  const uint32_t obs_small_width      = 60;
  const uint32_t obs_small_height     = 60;
  const uint32_t obstacle_spawn_rate  = 1; // Obstacles per second
  uint32_t       obstacle_spawn_timer = 0;

  // Init RNG engine seeded with time
  std::mt19937 rngEngine(static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));

  // Create distribution of ints
  std::uniform_int_distribution<std::size_t> spawnDistr(0, obs_short.size() - 1); // Careful! This is inclusive

  // Create Bernoulli (True or False) distribution with spawn chance
  float                       spawn_chance = 0.75;
  std::bernoulli_distribution flip_coin(spawn_chance);

  SetTraceLogLevel(LOG_ALL);

  while (!WindowShouldClose())
  {
    /**
     * @note Update logic goes before render! When rendering, we want the most
     * up-to-date game; that makes sense
     */

    // ===================================
    // Game Logic
    // ===================================

    if (IsKeyDown(KEY_SPACE))
    {
      niko.startJump();
    }

    // WARN: RESUME AT SETPOS GETTER

    niko.update();

    // Move the floor to create moving effect
    const float speed = 7;
    for (auto &x_pos : floor_chunk_x_positions)
    {
      x_pos -= speed;
      if (x_pos < -sand.width)
      {
        x_pos = SCREEN_WIDTH -
                (abs(x_pos) - abs(sand.width)); // Compensate for the amount of space we went over the shift point
      }
    }

    // Spawn obstacles
    obstacle_spawn_timer++;

    if (obstacle_spawn_timer >= (TARGET_FPS / obstacle_spawn_rate))
    {
      // Flip a coin; there is an x chance we will spawn
      bool should_spawn = flip_coin(rngEngine);
      if (should_spawn)
      {
        int       obstacle_texture   = static_cast<int>(spawnDistr(rngEngine));
        Rectangle obstacle_draw_rect = Rectangle{
            SCREEN_WIDTH + obs_small_width, FLOOR_Y_POS - obs_small_height, obs_small_width, obs_small_height};

        obstacles.push_back({obstacle_texture, obstacle_draw_rect});
      }

      obstacle_spawn_timer = 0;
    }

    // Move obstacles
    for (auto &obstacle : obstacles)
    {
      Rectangle &draw_dest = obstacle.second;
      draw_dest.x -= speed;
    }

    // Delete offscreen obstacles
    // NOTE: Lambda function in C++! Neat; the [&] specifies we capture args by reference
    obstacles.erase(std::remove_if(obstacles.begin(),
                                   obstacles.end(),
                                   [&](const auto &obstacle)
                                   {
                                     const Rectangle &draw_dest = obstacle.second;
                                     return draw_dest.x < -draw_dest.width;
                                   }),
                    obstacles.end());

    // ===================================
    // Game Rendering
    // ===================================

    BeginDrawing();

    ClearBackground(NK_BLUE);

    // Draw a bunch of fuckin clouds

    // Make a slightly transparent white to make the clouds fade off a bit
    const uint8_t faded_white_alpha = 50; // Out of 255
    Color         faded_white       = WHITE;
    faded_white.a                   = faded_white_alpha;

    // Draw each cloud at its specified pos
    for (const auto &pos : cloud_positions)
    {
      DrawTextureEx(cloud, pos, 0, 4, faded_white);
    }

    // Draw the sand floors
    for (const auto &x_pos : floor_chunk_x_positions)
    {
      DrawTexture(sand, x_pos, FLOOR_Y_POS, WHITE);
    }

    // Draw the obstacles
    for (const auto &obstacle : obstacles)
    {
      const int       &texture_index = obstacle.first;
      const Rectangle &draw_dest     = obstacle.second;

      DrawTextureEx(obs_short[texture_index],
                    {draw_dest.x, draw_dest.y},
                    0,
                    static_cast<float>(obs_small_width) / obs_short[texture_index].width,
                    WHITE);
    }

    GUI::drawTitle("Niko The\n\tNicotine-Addicted\n\t\tPunk Salamander", IMPACT_FONT, 32, 2, -90, true);

    niko.render();

    EndDrawing();
  }

  UnloadTexture(spr_niko_run); // TODO: Take care of these...
  UnloadTexture(spr_niko_jump);

  CloseWindow();

  return 0;
}
