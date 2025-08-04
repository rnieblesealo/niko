#include "constants.h"
#include "game-manager.h"
#include "gui.h"
#include "niko.h"
#include "scene.h"
#include "spritesheet-renderer.h"
#include <filesystem>
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

  const std::vector<Texture2D> obstacle_textures = {
      LoadTexture(std::filesystem::path("assets/tate.png").c_str()),
      LoadTexture(std::filesystem::path("assets/trump.png").c_str()),
      LoadTexture(std::filesystem::path("assets/netanyahu.png").c_str()),
  };

  GAME_MANAGER gameman(obstacle_textures);

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
    gameman.updateObstacles();

    // =====================================================================================
    // RENDERING
    // =====================================================================================

    BeginDrawing();
    ClearBackground(NK_BLUE);

    gameman.renderObstacles();

    /*
    GUI::drawTitle("Niko The\n\tNicotine-Addicted\n\t\tPunk Salamander",
                   IMPACT_FONT,
                   32,
                   2,
                   -90,
                   true);
    */

    // Draw some clouds; completely eyeballed!
    scene.drawStaticProp(cloud_texture, Vector2{130, 22}, 4, 0.3);
    scene.drawStaticProp(cloud_texture, Vector2{305, 70}, 3, 0.2);
    scene.drawStaticProp(cloud_texture, Vector2{-16, 127}, 2, 0.1);
    scene.render();

    niko.render(true);

    EndDrawing();
  }

  UnloadTexture(
      spr_niko_run); // TODO: Take care of these... You aren't freeing everything
  UnloadTexture(spr_niko_jump);

  CloseWindow();

  return 0;
}
