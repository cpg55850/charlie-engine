#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;
inline ComponentID getNewComponentTypeID() {
  static ComponentID lastID = 0u;
  return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
  static ComponentID typeID = getNewComponentTypeID();
  return typeID;
}

constexpr std::size_t maxComponents = 999999;
constexpr std::size_t maxGroups = 32;

using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
 public:
  Entity* entity;
  virtual void init() {}
  virtual void update(float deltaTime) {}
  virtual void draw() {}
  virtual ~Component() {}
};

class Entity {
 public:
  Entity(Manager& mManager) : manager(mManager), active(true) {
    componentArray.fill(nullptr);  // Ensure componentArray is initialized
    componentBitSet.reset();       // Reset componentBitSet
  }

  void update(float deltaTime) {
    if (!active) return;  // Ensure the entity is active
    for (auto& c : components) {
      if (c) c->update(deltaTime);
    }
  }

  void draw() {
    if (!active) return;  // Ensure the entity is active
    for (auto& c : components) {
      if (c) c->draw();
    }
  }

  bool isActive() const { return active; }
  void destroy() { active = false; }

  bool hasGroup(Group mGroup) const { return groupBitset[mGroup]; }

  void addGroup(Group mGroup);
  void delGroup(Group mGroup) { groupBitset[mGroup] = false; }

  template <typename T>
  bool hasComponent() const {
    return componentBitSet[getComponentTypeID<T>()];
  }

  template <typename T, typename... TArgs>
  T& addComponent(TArgs&&... mArgs) {
    auto c = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
    c->entity = this;
    T* rawPtr = c.get();
    components.emplace_back(std::move(c));
    componentArray[getComponentTypeID<T>()] = rawPtr;
    componentBitSet[getComponentTypeID<T>()] = true;
    rawPtr->init();
    return *rawPtr;
  }

  template <typename T>
  T& getComponent() const {
    T* ptr = static_cast<T*>(componentArray[getComponentTypeID<T>()]);
    if (!ptr) throw std::runtime_error("Component not found");
    return *ptr;
  }

 private:
  Manager& manager;
  bool active;
  std::vector<std::unique_ptr<Component>> components;
  ComponentArray componentArray;
  ComponentBitset componentBitSet;
  GroupBitset groupBitset;
};

class Manager {
 public:
  void update(float deltaTime) {
    for (auto& e : entities) {
      if (e) e->update(deltaTime);
    }
  }

  void draw() {
    for (auto& e : entities) {
      if (e) e->draw();
    }
  }

  void refresh() {
    for (auto i = 0u; i < maxGroups; i++) {
      auto& v = groupedEntities[i];
      v.erase(std::remove_if(v.begin(), v.end(),
                             [i](Entity* mEntity) {
                               return !mEntity->isActive() ||
                                      !mEntity->hasGroup(i);
                             }),
              v.end());
    }

    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [](const std::unique_ptr<Entity>& mEntity) {
                                    return !mEntity->isActive();
                                  }),
                   entities.end());
  }

  void addToGroup(Entity* mEntity, Group mGroup) {
    groupedEntities[mGroup].emplace_back(mEntity);
  }

  std::vector<Entity*>& getGroup(Group mGroup) {
    return groupedEntities[mGroup];
  }

  Entity& addEntity() {
    auto e = std::make_unique<Entity>(*this);
    Entity& ref = *e;
    entities.push_back(std::move(e));
    return ref;
  }

 private:
  std::array<std::vector<Entity*>, maxGroups> groupedEntities;
  std::vector<std::unique_ptr<Entity>> entities;
};