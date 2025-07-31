#include "constants.h"
#include "gui.h"
#include "niko.h"
#include "scene.h"
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
  // =========================================================================================
  // RAYLIB INIT
  // =========================================================================================

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE.c_str());
  InitAudioDevice();
  SetTargetFPS(TARGET_FPS);
  SetTraceLogLevel(LOG_ALL);

  // =========================================================================================
  // SETUP
  // =========================================================================================

  Font IMPACT_FONT = LoadFont("assets/impact.ttf");

  // ================================================================================
  // (PLAYER SETUP )
  // ================================================================================

  // Load spritesheet textures
  Texture2D spr_niko_run =
      LoadTexture(std::filesystem::path("assets/niko-run.png").c_str());
  Texture2D spr_niko_jump =
      LoadTexture(std::filesystem::path("assets/niko-jump.png").c_str());

  // Organize them into individual spritesheets
  std::map<std::string, SPRITESHEET const &> const &niko_spritesheets{
      {"run", SPRITESHEET{spr_niko_run, 8}}, {"jump", SPRITESHEET{spr_niko_jump, 1}}};

  // Create sprite renderer out of individual spritesheets
  std::shared_ptr<SPRITESHEET_RENDERER> niko_spritesheet_renderer(
      new SPRITESHEET_RENDERER(niko_spritesheets));

  // Apply some settings to renderer
  niko_spritesheet_renderer->setFPS(8);
  niko_spritesheet_renderer->enableOutline(true);
  niko_spritesheet_renderer->setSpritesheet("run");

  // Instantiate Niko himself and give him this renderer
  NIKO niko(niko_spritesheet_renderer);

  // Apply some settings to Niko
  niko.setPosition(100, FLOOR_Y_POS);

  // =========================================================================================
  // SCENE SETUP
  // =========================================================================================

  const Texture2D sand_floor_texture =
      LoadTexture(std::filesystem::path("assets/sand.png").c_str());

  const Texture2D cloud_texture =
      LoadTexture(std::filesystem::path("assets/cloud.png").c_str());

  SCENE scene(sand_floor_texture);

  // =====================================================================================
  // OBSTACLES SETUP
  // =====================================================================================

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
  std::mt19937 rngEngine(
      static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));

  // Create distribution of ints
  std::uniform_int_distribution<std::size_t> spawnDistr(
      0, obs_short.size() - 1); // Careful! This is inclusive

  // Create Bernoulli (True or False) distribution with spawn chance
  float                       spawn_chance = 0.75;
  std::bernoulli_distribution flip_coin(spawn_chance);

  // ======================================================================================
  // MAIN GAME LOOP
  // ======================================================================================

  while (!WindowShouldClose())
  {
    // =====================================================================================
    // UPDATE GAME LOGIC
    // =====================================================================================

    /**
     * @note We should always update logic before rendering! When rendering, we want the most
     * up-to-date game; that makes sense
     */

    // Scan for input to make player jump
    if (IsKeyDown(KEY_SPACE))
    {
      niko.startJump();
    }

    niko.update();
    scene.update();

    // Spawn obstacles
    obstacle_spawn_timer++;

    if (obstacle_spawn_timer >= (TARGET_FPS / obstacle_spawn_rate))
    {
      // Flip a coin; there is an x chance we will spawn
      bool should_spawn = flip_coin(rngEngine);
      if (should_spawn)
      {
        int       obstacle_texture   = static_cast<int>(spawnDistr(rngEngine));
        Rectangle obstacle_draw_rect = Rectangle{SCREEN_WIDTH + obs_small_width,
                                                 FLOOR_Y_POS - obs_small_height,
                                                 obs_small_width,
                                                 obs_small_height};

        obstacles.push_back({obstacle_texture, obstacle_draw_rect});
      }

      obstacle_spawn_timer = 0;
    }

    // Move obstacles
    for (auto &obstacle : obstacles)
    {
      Rectangle &draw_dest = obstacle.second;
      draw_dest.x -= GAME_SPEED;
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

    // =====================================================================================
    // RENDERING
    // =====================================================================================

    BeginDrawing();
    ClearBackground(NK_BLUE);

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

    GUI::drawTitle("Niko The\n\tNicotine-Addicted\n\t\tPunk Salamander",
                   IMPACT_FONT,
                   32,
                   2,
                   -90,
                   true);

    // Draw some clouds; completely eyeballed!
    scene.drawStaticProp(cloud_texture, Vector2{130, 22}, 4, 0.3);
    scene.drawStaticProp(cloud_texture, Vector2{305, 70}, 3, 0.2);
    scene.drawStaticProp(cloud_texture, Vector2{-16, 127}, 2, 0.1);
    scene.render();

    niko.render();

    EndDrawing();
  }

  UnloadTexture(
      spr_niko_run); // TODO: Take care of these... You aren't freeing everything
  UnloadTexture(spr_niko_jump);

  CloseWindow();

  return 0;
}
