#include "input-manager.h"
#include "game-object-intf.h"

INPUT_MANAGER::INPUT_MANAGER() {}

INPUT_MANAGER &INPUT_MANAGER::getInstance()
{
  static INPUT_MANAGER instance;
  return instance;
}

void INPUT_MANAGER::addObserver(GAME_OBJECT_INTF &observer)
{
  this->observers.push_back(observer);
}
