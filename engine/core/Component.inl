// Component.inl - template implementations that require a complete Entity type

#include "Component.hpp"
#include "Entity.hpp"

template<typename T>
T& Component::getComponent() {
    if (!entity) throw std::runtime_error("Component::getComponent: no owner entity");
    return entity->getComponent<T>();
}

template<typename T>
const T& Component::getComponent() const {
    if (!entity) throw std::runtime_error("Component::getComponent const: no owner entity");
    return entity->getComponent<T>();
}

