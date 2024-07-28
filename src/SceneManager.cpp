#include "SceneManager.hpp"

#include <iostream>

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
  auto it = scenes.find(sceneName);
  if (it != scenes.end()) {
    if (currentScene) {
      currentScene->onExit();
    }
    currentScene = it->second.get();
    currentScene->onEnter();
    std::cout << "Switched to scene " << sceneName << "." << std::endl;
  } else {
    std::cerr << "Scene " << sceneName << " not found." << std::endl;
  }
}

void SceneManager::update() {
  if (currentScene) {
    currentScene->update();
  }
}

void SceneManager::draw() {
  if (currentScene) {
    currentScene->draw();
  }
}

Scene* SceneManager::getCurrentScene() const { return currentScene; }
