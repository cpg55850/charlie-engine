#pragma once

#include "../core/Component.hpp"
#include <unordered_map>
#include <string>

// Component that stores input state for an entity
class InputComponent : public Component {
public:
    // Input states for this entity
    std::unordered_map<std::string, bool> pressedInputs;
    std::unordered_map<std::string, bool> justPressedInputs;
    std::unordered_map<std::string, bool> releasedInputs;

    InputComponent() = default;

    // Check if an input is currently pressed
    bool isPressed(const std::string& name) const {
        auto it = pressedInputs.find(name);
        return it != pressedInputs.end() && it->second;
    }

    // Check if an input was just pressed this frame
    bool isJustPressed(const std::string& name) const {
        auto it = justPressedInputs.find(name);
        return it != justPressedInputs.end() && it->second;
    }

    // Check if an input was just released this frame
    bool isReleased(const std::string& name) const {
        auto it = releasedInputs.find(name);
        return it != releasedInputs.end() && it->second;
    }

    // Helper to set input state (used by InputSystem)
    void setInput(const std::string& name, bool pressed) {
        pressedInputs[name] = pressed;
    }
};

