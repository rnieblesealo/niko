#ifndef GAME_OBJECT_INTF_H
#define GAME_OBJECT_INTF_H

#include "types.h"
#include <memory>

/**
 * @brief Contract for something that's a game object
 * i.e. It re-renders and updates each frame
 * */
class GAME_OBJECT_INTF
{
protected:
  GAME_OBJECT_INTF() = default;

public:
  // NOTE: Virtual destructor ensures when child class is deleted, its inherited destructor is called first
  virtual ~GAME_OBJECT_INTF() = default;

  /**
   * @brief Draws this object to the screen
   * @param debug_mode Should this render visual debugging?
   */
  virtual void render(bool debug_mode) = 0;

  /**
   * @brief Updates this object's state
   * State updates should always take place before drawing!
   */
  virtual void update() = 0;

  /**
   * @brief Updates this object's state
   * State updates should always take place before drawing!
   */
  virtual void onStateChangedTo(GAME_STATE state) = 0;

  /**
   * @brief Equality for 2 game objects means they have the same address; ergo, they are literally the same object!
   */
  bool operator==(const GAME_OBJECT_INTF &other) { return this == std::addressof(other); }
};

#endif
