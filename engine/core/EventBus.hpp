#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>

namespace engine::events {

// Base polymorphic event
struct Event {
    virtual ~Event() = default;
};
using EventPtr = std::shared_ptr<Event>;

// Example high-level event: a collision occurred between two entities
struct CollisionOccurred : public Event {
    class Entity* a = nullptr;
    class Entity* b = nullptr;
    CollisionOccurred(class Entity* a_, class Entity* b_) : a(a_), b(b_) {}
};

// Example high-level event: an entity was hit (game semantics can use this)
struct OnHitEvent : public Event {
    class Entity* target = nullptr;
    class Entity* source = nullptr; // could be nullptr for environment
    int damage = 0;
    OnHitEvent(class Entity* tgt, class Entity* src, int dmg) : target(tgt), source(src), damage(dmg) {}
};

class EventBus {
public:
    using RawHandler = std::function<void(EventPtr)>;

    // Subscribe with handler specific to EventType. The handler receives shared_ptr<EventType>
    template<typename EventType>
    void subscribe(std::function<void(std::shared_ptr<EventType>)> handler) {
        auto idx = std::type_index(typeid(EventType));
        RawHandler wrapper = [handler](EventPtr base) {
            handler(std::static_pointer_cast<EventType>(base));
        };
        subscribers[idx].push_back(std::move(wrapper));
    }

    // Publish an already created EventPtr (derived type)
    template<typename EventType>
    void publish(std::shared_ptr<EventType> ev) {
        auto idx = std::type_index(typeid(EventType));
        auto it = subscribers.find(idx);
        if (it == subscribers.end()) return;
        for (auto &h : it->second) {
            h(ev);
        }
    }

    // Convenience: create and publish in one call
    template<typename EventType, typename... Args>
    void publishEvent(Args&&... args) {
        auto ev = std::make_shared<EventType>(std::forward<Args>(args)...);
        publish<EventType>(ev);
    }

private:
    std::unordered_map<std::type_index, std::vector<RawHandler>> subscribers;
};

// Global accessor for the engine event bus (single instance)
inline EventBus & GetEventBus() {
    static EventBus bus;
    return bus;
}

} // namespace engine::events

