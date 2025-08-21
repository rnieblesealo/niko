#include "input-manager.h"

INPUT_MANAGER::INPUT_MANAGER() {}

INPUT_MANAGER &INPUT_MANAGER::getInstance()
{
  static INPUT_MANAGER instance;
  return instance;
}
