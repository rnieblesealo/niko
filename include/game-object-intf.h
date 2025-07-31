#ifndef GAME_OBJECT_INTF_H
#define GAME_OBJECT_INTF_H

/**
 * @brief Contract for something that's a game object
 * A game object is something that's instantiated, is drawable, and needs to update in line with the game's state
 * */
class GAME_OBJECT_INTF
{
protected:
  GAME_OBJECT_INTF() = default;

public:
  // NOTE: Virtual destructor ensures when child class is deleted, its constructor is called first
  virtual ~GAME_OBJECT_INTF() = default;

  /**
   * @brief Draws this object to the screen
   * The parameters we use to do so might vary (Will we use a dest rect? Position? etc...)
   */
  virtual void render() = 0;

  /**
   * @brief Updates this object's state
   * State updates should always take place before drawing!
   */
  virtual void update() = 0;
};

#endif
