#include "constants.h"
#include "game-manager.h"
#include "gui.h"
#include "niko.h"
#include "scene.h"
#include "spritesheet-renderer.h"
#include <cstdlib>
#include <filesystem>
#include <raylib.h>

using namespace COLORPAL_12J4NK;

int main(void)
{
  // =========================================================================================
  // RAYLIB INIT
  // =========================================================================================

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE.c_str());
  SetTargetFPS(TARGET_FPS);
  InitAudioDevice();
  SetTraceLogLevel(LOG_ALL);

  // =========================================================================================
  // SETUP
  // =========================================================================================

  Font IMPACT_FONT = LoadFont("assets/impact.ttf");

  // ================================================================================
  // PLAYER SETUP
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
  niko.setPosition(PLAYER_X_POS, FLOOR_Y_POS);

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

  const std::vector<std::shared_ptr<Texture2D>> obstacle_textures = {
      std::make_shared<Texture2D>(
          LoadTexture(std::filesystem::path("assets/tate.png").c_str())),
      std::make_shared<Texture2D>(
          LoadTexture(std::filesystem::path("assets/trump.png").c_str())),
      std::make_shared<Texture2D>(
          LoadTexture(std::filesystem::path("assets/netanyahu.png").c_str())),
  };

  // ======================================================================================
  // MAIN GAME LOOP
  // ======================================================================================

  while (!WindowShouldClose())
  {
    // =====================================================================================
    // UPDATE GAME LOGIC
    // =====================================================================================

    niko.update();

    if (niko.isTouchingAny(GAME_MANAGER::getInstance().getActiveObstacles()) &&
        // Only register collisions if in game
        GAME_MANAGER::getInstance().getCurrentState() == GAME_STATE::IN_GAME)
    {
      GAME_MANAGER::getInstance().advanceState();
    }

    scene.update();

    // Only spawn obstacles in regular gameplay
    if (GAME_MANAGER::getInstance().getCurrentState() == GAME_STATE::IN_GAME)
    {
      GAME_MANAGER::getInstance().moveActiveObstacles();
      GAME_MANAGER::getInstance().runObstacleSpawner(obstacle_textures);

      // TODO: Resume here; need to clear all obstacles and reset Niko when switching state!
      // You might also have a circular dep here (gameman must call gameobjects to run cleanup, but they also depend on gameman to check state...)
      // Suggest using observer pattern with GAME_OBJECT contract!
    }

    // But always handle offscreen obstacles as safety!
    GAME_MANAGER::getInstance().removeOffscreenObstacles();

    // =====================================================================================
    // RENDERING
    // =====================================================================================

    BeginDrawing();
    ClearBackground(NK_BLUE);

    GAME_MANAGER::getInstance().renderObstacles();

    // Draw game over/title
    if (GAME_MANAGER::getInstance().getCurrentState() == GAME_STATE::TITLE)
    {
      GUI::drawBigText("Niko The\n\tNicotine-Addicted\n\t\tPunk Salamander", IMPACT_FONT);
    }
    else if (GAME_MANAGER::getInstance().getCurrentState() == GAME_STATE::GAME_OVER)
    {
      GUI::drawBigText("GAME OVER!", IMPACT_FONT, 54, 2, 0, NK_RED, true, 3);
    }

    // Draw some clouds; completely eyeballed!
    scene.drawStaticProp(cloud_texture, Vector2{130, 22}, 4, 0.3);
    scene.drawStaticProp(cloud_texture, Vector2{305, 70}, 3, 0.2);
    scene.drawStaticProp(cloud_texture, Vector2{-16, 127}, 2, 0.1);
    scene.render();

    niko.render();

    EndDrawing();
  }

  // FIXME: No alloc'd raylib resources are being freed correctly!
  UnloadTexture(spr_niko_run);
  UnloadTexture(spr_niko_jump);
  // Add the rest...

  CloseWindow();

  return 0;
}
