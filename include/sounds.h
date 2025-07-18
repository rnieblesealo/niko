#include <filesystem>
#include <raylib.h>

#ifndef SOUNDS_H
#define SOUNDS_H

namespace SOUNDS
{
const Sound SFX_JUMP = LoadSound(std::filesystem::path("assets/jump.wav").c_str());
const Sound SFX_LAND = LoadSound(std::filesystem::path("assets/land.wav").c_str());
} // namespace SOUNDS

#endif
