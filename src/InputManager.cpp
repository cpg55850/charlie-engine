#include "InputManager.hpp"

#include <SDL.h>

#include <cstring>

InputManager::InputManager() {
  keyboardState = new Uint8[SDL_NUM_SCANCODES];
  previousKeyboardState = new Uint8[SDL_NUM_SCANCODES];
  std::memset(keyboardState, 0, SDL_NUM_SCANCODES);
  std::memset(previousKeyboardState, 0, SDL_NUM_SCANCODES);
}

InputManager::~InputManager() {
  delete[] keyboardState;
  delete[] previousKeyboardState;
}

void InputManager::addInput(const std::string& name,
                            const std::vector<SDL_Scancode>& scancodes) {
  inputMap[name] = scancodes;
}

bool InputManager::isPressed(const std::string& name) const {
  auto it = inputMap.find(name);
  if (it != inputMap.end()) {
    for (const auto& scancode : it->second) {
      if (scancode < SDL_NUM_SCANCODES && keyboardState[scancode]) {
        return true;
      }
    }
  }
  return false;
}

bool InputManager::isJustPressed(const std::string& name) const {
  auto it = inputMap.find(name);
  if (it != inputMap.end()) {
    for (const auto& scancode : it->second) {
      if (scancode < SDL_NUM_SCANCODES && keyboardState[scancode] &&
          !previousKeyboardState[scancode]) {
        return true;
      }
    }
  }
  return false;
}

bool InputManager::isReleased(const std::string& name) const {
  auto it = inputMap.find(name);
  if (it != inputMap.end()) {
    for (const auto& scancode : it->second) {
      if (scancode < SDL_NUM_SCANCODES && !keyboardState[scancode] &&
          previousKeyboardState[scancode]) {
        return true;
      }
    }
  }
  return false;
}

void InputManager::update() {
  std::memcpy(previousKeyboardState, keyboardState, SDL_NUM_SCANCODES);
  const Uint8* currentState = SDL_GetKeyboardState(NULL);
  std::memcpy(keyboardState, currentState, SDL_NUM_SCANCODES);
}