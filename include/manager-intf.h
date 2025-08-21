#ifndef MANAGER_INTF_H
#define MANAGER_INTF_H

#include "vector"

class GAME_OBJECT_INTF;

class MANAGER_INTF
{
private:
  MANAGER_INTF() = default;

  /**
   * @brief List of current observers
   */
  std::vector<std::reference_wrapper<GAME_OBJECT_INTF>> observers{};

public:
  virtual ~MANAGER_INTF() = default;

  // WARNING: I'm not sure this constructor handling will enforce in child classes (since this' constructor is private), investigate
  MANAGER_INTF(MANAGER_INTF const &) = delete; // No copies (why copy a SINGLEton?)
  void operator=(MANAGER_INTF const &) =
      delete; // Cannot check for equality (why compare a SINGLEton?)

  /**
   * @brief Adds an observer
   */
  virtual void addObserver(GAME_OBJECT_INTF &observer) = 0;
};

#endif
