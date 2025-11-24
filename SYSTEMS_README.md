# ECS Systems Documentation

## Overview

This project uses an Entity-Component-System (ECS) architecture where:
- **Entities** are containers with unique IDs
- **Components** hold data (TransformComponent, SpriteComponent, etc.)
- **Systems** contain logic that operates on entities with specific components

## Available Systems

### 1. System (Base Class)
**Location:** `include/ECS/System.hpp`

Base class for all systems. Defines the interface that all systems must implement.

```cpp
class System {
public:
    virtual void update(Manager& manager, float deltaTime) = 0;
    virtual void init() {}
};
```

### 2. MovementSystem
**Location:** `include/ECS/MovementSystem.hpp`

Updates entity positions based on their velocity.

**Processes:**
- Entities with `TransformComponent`

**Behavior:**
```cpp
position.x += velocity.x * deltaTime;
position.y += velocity.y * deltaTime;
```

**Example:**
```cpp
auto& entity = manager.addEntity();
auto& transform = entity.addComponent<TransformComponent>();
transform.velocity.x = 100.0f; // Move right at 100 units/sec
transform.velocity.y = 50.0f;  // Move down at 50 units/sec
```

### 3. RenderSystem
**Location:** `include/ECS/RenderSystem.hpp`

Renders all visual components in the correct order.

**Processes:**
- `TileComponent` (background layer)
- `SpriteComponent` (static sprites)
- `AnimatedSpriteComponent` (animated sprites)
- `ColliderComponent` (debug mode only with `DEBUG_COLLIDERS` defined)

**Rendering Order:**
1. Tiles (background)
2. Static sprites
3. Animated sprites
4. Debug colliders (optional)

**Example:**
```cpp
auto& entity = manager.addEntity();
entity.addComponent<TransformComponent>(100, 100, 32, 32, 2);
entity.addComponent<SpriteComponent>("assets/player.png");
// RenderSystem will automatically draw this entity
```

### 4. AnimationSystem
**Location:** `include/ECS/AnimationSystem.hpp`

Updates sprite animations based on time and animation settings.

**Processes:**
- Entities with `AnimatedSpriteComponent`

**Behavior:**
- Updates animation frames
- Handles animation speed
- Switches between animation states

**Example:**
```cpp
auto& entity = manager.addEntity();
entity.addComponent<TransformComponent>(200, 200, 16, 16, 4);
auto& animSprite = entity.addComponent<AnimatedSpriteComponent>();
animSprite.addTex("assets/walk-right.png", Animation(0, 4, 100));
animSprite.playTex("assets/walk-right.png");
```

### 5. CollisionSystem
**Location:** `include/ECS/CollisionSystem.hpp`

Detects collisions between entities with colliders.

**Processes:**
- Entities with `ColliderComponent`

**Behavior:**
- Checks all pairs of entities for AABB collisions
- Calls `handleCollision()` when colliders overlap
- Collision response should be implemented in script components

**Example:**
```cpp
auto& player = manager.addEntity();
player.addComponent<TransformComponent>(100, 100, 16, 16, 4);
player.addComponent<ColliderComponent>("player");

auto& wall = manager.addEntity();
wall.addComponent<TransformComponent>(150, 100, 32, 32, 1);
wall.addComponent<ColliderComponent>("wall");
// CollisionSystem will detect when these overlap
```

### 6. ScriptSystem
**Location:** `include/ECS/ScriptSystem.hpp`

Executes custom game logic defined in script components.

**Processes:**
- Entities with `ScriptComponent` or derived classes
- Examples: `PlayerComponent`, `EnemyComponent`, `BulletComponent`

**Behavior:**
- Calls `update()` on each script component
- Allows custom per-entity behavior

**Example:**
```cpp
auto& player = manager.addEntity();
player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
player.addComponent<PlayerComponent>(); // Custom script
// ScriptSystem will call PlayerComponent::update() each frame
```

## System Setup

### Adding Systems to Manager

Systems should be added during initialization (typically in `Game::init()` or `Scene::onEnter()`):

```cpp
void Game::init() {
    // Add systems in the order they should execute
    manager.addSystem<ScriptSystem>();      // 1. Custom logic
    manager.addSystem<MovementSystem>();    // 2. Physics/movement
    manager.addSystem<AnimationSystem>();   // 3. Animations
    manager.addSystem<CollisionSystem>();   // 4. Collision detection
    manager.addSystem<RenderSystem>();      // 5. Rendering (last)
}
```

### System Execution Order

Systems are executed in the order they are added. Recommended order:

1. **ScriptSystem** - Custom game logic (input handling, AI)
2. **MovementSystem** - Position updates based on velocity
3. **AnimationSystem** - Animation frame updates
4. **CollisionSystem** - Collision detection
5. **RenderSystem** - Draw everything (should be last)

### Automatic Updates

The Manager automatically updates all systems:

```cpp
void Game::update(float deltaTime) {
    manager.refresh();              // Remove inactive entities
    manager.update(deltaTime);      // Updates all systems
}
```

## Creating System-Compatible Entities

### Simple Static Sprite
```cpp
auto& entity = manager.addEntity();
entity.addComponent<TransformComponent>(100, 100, 32, 32, 2);
entity.addComponent<SpriteComponent>("assets/sprite.png");
// Rendered by RenderSystem
```

### Moving Entity
```cpp
auto& entity = manager.addEntity();
auto& transform = entity.addComponent<TransformComponent>(0, 0, 16, 16, 4);
transform.velocity.x = 100.0f; // Velocity set by script or input
entity.addComponent<SpriteComponent>("assets/entity.png");
// Updated by MovementSystem, rendered by RenderSystem
```

### Animated Character
```cpp
auto& player = manager.addEntity();
player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
auto& anim = player.addComponent<AnimatedSpriteComponent>();
anim.addTex("assets/walk-right.png", Animation(0, 4, 100));
anim.addTex("assets/walk-left.png", Animation(0, 4, 100));
anim.playTex("assets/walk-right.png");
// Updated by AnimationSystem, rendered by RenderSystem
```

### Collidable Entity
```cpp
auto& entity = manager.addEntity();
entity.addComponent<TransformComponent>(200, 200, 32, 32, 1);
entity.addComponent<SpriteComponent>("assets/box.png");
entity.addComponent<ColliderComponent>("box");
// Collision checked by CollisionSystem
```

### Fully Featured Entity
```cpp
auto& player = manager.addEntity();
player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
player.addComponent<AnimatedSpriteComponent>("assets/player.png", Animation(0, 4, 100));
player.addComponent<ColliderComponent>("player");
player.addComponent<PlayerComponent>(); // Custom script
player.addGroup(Game::groupPlayers);

// This entity:
// - Has custom logic (ScriptSystem)
// - Moves (MovementSystem)
// - Is animated (AnimationSystem)
// - Collides with things (CollisionSystem)
// - Is rendered (RenderSystem)
```

## Advanced Usage

### Accessing a Specific System
```cpp
auto& renderSystem = manager.getSystem<RenderSystem>();
// Call system-specific methods if you add any
```

### Entity Groups with Systems
```cpp
// Systems automatically process all entities
// But you can also manually process groups:
for (auto& entity : manager.getGroup(Game::groupPlayers)) {
    if (entity->hasComponent<TransformComponent>()) {
        // Do something with player entities
    }
}
```

### Custom Systems

Create your own system by inheriting from `System`:

```cpp
class MyCustomSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;
            
            // Process entities with specific components
            if (entity->hasComponent<MyComponent>()) {
                auto& comp = entity->getComponent<MyComponent>();
                // Update logic here
            }
        }
    }
};

// Add it to the manager:
manager.addSystem<MyCustomSystem>();
```

## Performance Considerations

1. **System Order Matters** - Put frequently-updated systems first
2. **Component Checks** - Systems only process entities with required components
3. **Entity Refresh** - Call `manager.refresh()` regularly to remove inactive entities
4. **Use Groups** - For targeted processing of specific entity types
5. **Batch Operations** - Systems process all matching entities in one pass

## Debugging

### Enable Collider Visualization
```cpp
// In your build configuration, define:
#define DEBUG_COLLIDERS
// This will show collision boxes in RenderSystem
```

### Check System Execution
```cpp
void MySystem::update(Manager& manager, float deltaTime) {
    std::cout << "MySystem processing " 
              << manager.getEntities().size() 
              << " entities" << std::endl;
    // ...
}
```

## Files

- `include/ECS/System.hpp` - Base system class
- `include/ECS/MovementSystem.hpp` - Movement logic
- `include/ECS/RenderSystem.hpp` - Rendering logic
- `include/ECS/AnimationSystem.hpp` - Animation logic
- `include/ECS/CollisionSystem.hpp` - Collision detection
- `include/ECS/ScriptSystem.hpp` - Script execution
- `include/ECS/Systems.hpp` - Convenience header including all systems
- `include/ECS/SYSTEMS_USAGE.cpp` - Example code
- `src/ECS/Manager.cpp` - System management implementation

## See Also

- `Components.hpp` - Available components
- `Manager.hpp` - Entity and system management
- `Entity.hpp` - Entity implementation

