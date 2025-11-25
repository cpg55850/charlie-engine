#pragma once

// Entity template method implementations
// Separated to avoid circular dependency with Manager

template <typename T>
bool Entity::hasComponent() const {
    return componentBitSet[getComponentTypeID<T>()];
}

template <typename T, typename... TArgs>
T& Entity::addComponent(TArgs&&... mArgs) {
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
T& Entity::getComponent() const {
    T* ptr = static_cast<T*>(componentArray[getComponentTypeID<T>()]);
    if (!ptr) throw std::runtime_error("Component not found");
    return *ptr;
}
