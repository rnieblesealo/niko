#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

class INPUT_MANAGER final
{
private:
  INPUT_MANAGER();

public:
  static INPUT_MANAGER &getInstance();

  INPUT_MANAGER(INPUT_MANAGER const &)  = delete; // No copies
  void operator=(INPUT_MANAGER const &) = delete; // Cannot check for equality
};

#endif
