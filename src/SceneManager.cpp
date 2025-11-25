#include "SceneManager.hpp"

#include <iostream>

#include "Game.hpp"
#include "SceneFactory.hpp"

void SceneManager::loadScene(const std::string& sceneName) {
  if (scenes.find(sceneName) == scenes.end()) {
    auto scene = SceneFactory::instance().createScene(sceneName);
    if (scene) {
      scenes[sceneName] = std::move(scene);
      std::cout << "Scene " << sceneName << " loaded." << std::endl;
    } else {
      std::cerr << "Scene " << sceneName << " not registered." << std::endl;
    }
  }
}

void SceneManager::switchScene(const std::string& sceneName) {
  // If scene not loaded yet, attempt to load via factory automatically
  auto it = scenes.find(sceneName);
  if (it == scenes.end()) {
    auto created = SceneFactory::instance().createScene(sceneName);
    if (!created) {
      std::cerr << "Scene " << sceneName << " not registered." << std::endl;
      return;
    }
    scenes[sceneName] = std::move(created);
    it = scenes.find(sceneName);
    std::cout << "Scene " << sceneName << " loaded." << std::endl;
  }

  // Avoid re-entering the same active scene
  if (currentScene == it->second.get()) {
    std::cout << "Scene " << sceneName << " already active; switch skipped." << std::endl;
    return;
  }

  if (currentScene) {
    currentScene->onExit();
  }
  currentScene = it->second.get();
  currentScene->onEnter();
  std::cout << "Switched to scene " << sceneName << "." << std::endl;
}

void SceneManager::update(float deltaTime) {
  Game::manager.update(deltaTime);  // InputSystem handles input now
  if (currentScene) {
    currentScene->update(deltaTime);
  }
}

void SceneManager::draw() {
  if (currentScene) {
    currentScene->draw();
  }
}

Scene* SceneManager::getCurrentScene() const { return currentScene; }
