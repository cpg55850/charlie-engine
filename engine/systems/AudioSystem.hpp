#pragma once

#include "System.hpp"
#include "../core/Manager.hpp"
#include "../../include/scripts/Components.hpp"
#include "../../include/Game.hpp"

// Component for entities that can play audio
class AudioComponent : public Component {
public:
    std::string audioKey;
    bool autoPlay = false;
    bool loop = false;
    int channel = -1;
    bool playOnce = false;

    AudioComponent() = default;
    AudioComponent(const std::string& key, bool autoplay = false, bool looping = false)
        : audioKey(key), autoPlay(autoplay), loop(looping) {}

    void init() override {
        if (autoPlay && !playOnce) {
            play();
            playOnce = true;
        }
    }

    void play() {
        if (!audioKey.empty()) {
            Game::audioManager.playAudio(audioKey, loop ? -1 : 0, channel);
        }
    }

    void setAudio(const std::string& key) {
        audioKey = key;
    }
};

// System that handles audio playback for entities with AudioComponent
class AudioSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Process entities with AudioComponent
            if (entity->hasComponent<AudioComponent>()) {
                auto& audio = entity->getComponent<AudioComponent>();
                // Audio components are typically triggered by scripts
                // This system ensures autoPlay works on init
            }
        }
    }
};

