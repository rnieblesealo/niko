#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <vector>

class GAME_OBJECT_INTF;

class INPUT_MANAGER final
{
private:
  INPUT_MANAGER();

  /**
   * @brief List of current observers
   */
  std::vector<std::reference_wrapper<GAME_OBJECT_INTF>> observers{};

public:
  static INPUT_MANAGER &getInstance();

  INPUT_MANAGER(INPUT_MANAGER const &)  = delete; // No copies
  void operator=(INPUT_MANAGER const &) = delete; // Cannot check for equality

  /**
   * @brief Adds an observer
   */
  void addObserver(GAME_OBJECT_INTF &observer);
};

#endif
