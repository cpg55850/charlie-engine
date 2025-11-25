#include <SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

class InputManager {
 public:
  InputManager();
  ~InputManager();

  void addInput(const std::string& name,
                const std::vector<SDL_Scancode>& scancodes);
  bool isPressed(const std::string& name) const;
  bool isJustPressed(const std::string& name) const;
  bool isReleased(const std::string& name) const;
  void update();

 private:
  Uint8* keyboardState;
  Uint8* previousKeyboardState;
  std::unordered_map<std::string, std::vector<SDL_Scancode>> inputMap;
};