#include "ECS/ECS.hpp"

#include <iostream>

void Entity::addGroup(Group mGroup) {
  // std::cout << "Reached Entity::addGroup" << std::endl;
  groupBitset[mGroup] = true;
  manager.addToGroup(this, mGroup);
}