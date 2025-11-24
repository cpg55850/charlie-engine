# ECS System Guide for Your Game Engine

## Table of Contents
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Components](#components)
4. [Systems](#systems)
5. [Entities](#entities)
6. [Quick Start Guide](#quick-start-guide)
7. [Complete Examples](#complete-examples)
8. [Best Practices](#best-practices)
9. [API Reference](#api-reference)

---

## Overview

Your game engine uses an **Entity-Component-System (ECS)** architecture, which provides:
- **Modularity**: Easy to add/remove features
- **Performance**: Systems process entities in batches
- **Flexibility**: Entities are defined by composition, not inheritance
- **Maintainability**: Clear separation between data and logic

### What is ECS?

- **Entities**: Simple containers with unique IDs (like game objects)
- **Components**: Pure data (position, sprite, health, etc.)
- **Systems**: Logic that operates on entities with specific components

---

## Architecture

```
┌─────────────────────────────────────────────────┐
│                   Manager                        │
│  - Owns all entities                            │
│  - Manages systems                              │
│  - Handles entity lifecycle                     │
└─────────────────────────────────────────────────┘
                    │
        ┌───────────┴───────────┐
        │                       │
   ┌────▼─────┐          ┌─────▼──────┐
   │ Entities │          │  Systems   │
   └────┬─────┘          └─────┬──────┘
        │                      │
   ┌────▼─────┐          ┌─────▼──────┐
   │Components│◄─────────┤  Process   │
   │  (Data)  │          │ Components │
   └──────────┘          └────────────┘
```

### Key Classes

#### Entity (`Entity.hpp`)
```cpp
class Entity {
    // Add components to entity
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs);
    
    // Check if entity has component
    template <typename T>
    bool hasComponent() const;
    
    // Get component reference
    template <typename T>
    T& getComponent() const;
    
    // Group management
    void addGroup(Group group);
    bool hasGroup(Group group) const;
    
    // Lifecycle
    void destroy();
    bool isActive() const;
};
```

#### Manager (`Manager.hpp`)
```cpp
class Manager {
    // Entity management
    Entity& addEntity();
    void refresh(); // Remove inactive entities
    std::vector<Entity*>& getGroup(Group group);
    
    // System management
    template <typename T, typename... TArgs>
    T& addSystem(TArgs&&... mArgs);
    
    template <typename T>
    T& getSystem();
    
    // Update all systems
    void update(float deltaTime);
};
```

#### Component (`Component.hpp`)
```cpp
class Component {
    Entity* entity; // Reference to owning entity
    
    virtual void init() {}
    virtual void update(float deltaTime) {}
    virtual void draw() {}
};
```

#### System (`System.hpp`)
```cpp
class System {
    virtual void update(Manager& manager, float deltaTime) = 0;
    virtual void init() {}
};
```

---

## Components

Components are pure data holders. Here are all available components:

### Core Components

#### TransformComponent
**Purpose**: Position, scale, and velocity  
**File**: `include/ECS/TransformComponent.hpp`

```cpp
struct TransformComponent {
    Vector2D position;
    Vector2D velocity;
    int width = 16;
    int height = 16;
    int scale = 1;
};
```

**Usage**:
```cpp
auto& entity = manager.addEntity();
auto& transform = entity.addComponent<TransformComponent>(x, y, width, height, scale);
transform.velocity.x = 100.0f; // Set velocity
```

---

### Visual Components

#### SpriteComponent
**Purpose**: Static sprite rendering  
**File**: `include/ECS/SpriteComponent.hpp`

```cpp
entity.addComponent<SpriteComponent>("assets/player.png");
```

#### AnimatedSpriteComponent
**Purpose**: Animated sprite with multiple frames  
**File**: `include/ECS/AnimatedSpriteComponent.hpp`

```cpp
auto& anim = entity.addComponent<AnimatedSpriteComponent>();
anim.addTex("assets/walk-right.png", Animation(0, 4, 100)); // 4 frames, 100ms speed
anim.addTex("assets/walk-left.png", Animation(0, 4, 100));
anim.playTex("assets/walk-right.png");
```

#### TileComponent
**Purpose**: Tilemap rendering  
**File**: `include/ECS/TileComponent.hpp`

```cpp
entity.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileset);
```

---

### Physics Components

#### ColliderComponent
**Purpose**: Collision detection  
**File**: `include/ECS/ColliderComponent.hpp`

```cpp
entity.addComponent<ColliderComponent>("player"); // Tag for identification
```

#### PhysicsComponent
**Purpose**: Physics properties (gravity, friction, etc.)  
**File**: `include/ECS/PhysicsSystem.hpp`

```cpp
auto& physics = entity.addComponent<PhysicsComponent>(1.0f, true); // mass, useGravity
physics.friction = 0.95f;
physics.bounciness = 0.3f;
physics.gravityScale = 1.0f;
```

---

### Script Components

#### ScriptComponent
**Purpose**: Custom entity behavior  
**File**: `include/ECS/ScriptComponent.hpp`

Create custom behaviors by extending ScriptComponent:
```cpp
class PlayerComponent : public ScriptComponent {
    void init() override {
        // Setup player
    }
    
    void update(float deltaTime) override {
        // Handle player input and logic
    }
};
```

---

### Audio Components

#### AudioComponent
**Purpose**: Sound playback for entities  
**File**: `include/ECS/AudioSystem.hpp`

```cpp
auto& audio = entity.addComponent<AudioComponent>("laser", true, false);
// audioKey, autoPlay, loop
audio.play(); // Manually play sound
```

---

### UI Components

#### ButtonComponent
**Purpose**: Interactive buttons  
**File**: `include/ECS/ButtonComponent.hpp`

```cpp
entity.addComponent<ButtonComponent>();
```

#### LabelComponent
**Purpose**: Text rendering  
**File**: `include/ECS/LabelComponent.hpp`

```cpp
entity.addComponent<LabelComponent>();
```

---

## Systems

Systems contain the game logic that operates on components. They execute in the order they're added.

### System Execution Flow

```
┌──────────────┐
│ InputSystem  │ ← Handle input first
├──────────────┤
│ ScriptSystem │ ← Custom game logic
├──────────────┤
│PhysicsSystem │ ← Apply physics forces
├──────────────┤
│MovementSystem│ ← Update positions
├──────────────┤
│AnimationSys  │ ← Update animations
├──────────────┤
│CollisionSys  │ ← Detect collisions
├──────────────┤
│ AudioSystem  │ ← Handle audio
├──────────────┤
│ RenderSystem │ ← Draw everything (last!)
└──────────────┘
```

### Available Systems

#### 1. InputSystem
**File**: `include/ECS/InputSystem.hpp`  
**Purpose**: Updates input state  
**Processes**: N/A (updates global InputManager)

```cpp
manager.addSystem<InputSystem>();
// Scripts can then use: Game::inputManager.isPressed("MoveRight")
```

#### 2. ScriptSystem
**File**: `include/ECS/ScriptSystem.hpp`  
**Purpose**: Executes custom entity logic  
**Processes**: Entities with ScriptComponent (or derived)

```cpp
manager.addSystem<ScriptSystem>();
```

#### 3. PhysicsSystem
**File**: `include/ECS/PhysicsSystem.hpp`  
**Purpose**: Applies physics forces  
**Processes**: Entities with PhysicsComponent + TransformComponent

```cpp
auto& physics = manager.addSystem<PhysicsSystem>();
physics.gravity = 980.0f; // Adjust gravity
```

#### 4. MovementSystem
**File**: `include/ECS/MovementSystem.hpp`  
**Purpose**: Updates positions based on velocity  
**Processes**: Entities with TransformComponent

```cpp
manager.addSystem<MovementSystem>();
```

#### 5. AnimationSystem
**File**: `include/ECS/AnimationSystem.hpp`  
**Purpose**: Updates animation frames  
**Processes**: Entities with AnimatedSpriteComponent

```cpp
manager.addSystem<AnimationSystem>();
```

#### 6. CollisionSystem
**File**: `include/ECS/CollisionSystem.hpp`  
**Purpose**: Detects AABB collisions  
**Processes**: Entities with ColliderComponent

```cpp
manager.addSystem<CollisionSystem>();
```

#### 7. AudioSystem
**File**: `include/ECS/AudioSystem.hpp`  
**Purpose**: Handles audio playback  
**Processes**: Entities with AudioComponent

```cpp
manager.addSystem<AudioSystem>();
```

#### 8. RenderSystem
**File**: `include/ECS/RenderSystem.hpp`  
**Purpose**: Renders all visual components  
**Processes**: Entities with Sprite/AnimatedSprite/Tile components

```cpp
manager.addSystem<RenderSystem>();
```

---

## Entities

Entities are created through the Manager and are defined by their components.

### Entity Groups

Entities can belong to groups for efficient querying:

```cpp
// In Game.hpp
enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupProjectiles
};

// Add entity to group
entity.addGroup(Game::groupPlayers);

// Query group
auto& players = manager.getGroup(Game::groupPlayers);
for (auto* player : players) {
    // Process player entities
}
```

---

## Quick Start Guide

### Step 1: Initialize Systems

In your `Game::init()` or `Scene::onEnter()`:

```cpp
void Game::init() {
    // Add systems in execution order
    manager.addSystem<InputSystem>();
    manager.addSystem<ScriptSystem>();
    manager.addSystem<PhysicsSystem>();
    manager.addSystem<MovementSystem>();
    manager.addSystem<AnimationSystem>();
    manager.addSystem<CollisionSystem>();
    manager.addSystem<AudioSystem>();
    manager.addSystem<RenderSystem>();
}
```

### Step 2: Create Entities

```cpp
// Create a simple entity
auto& entity = manager.addEntity();
entity.addComponent<TransformComponent>(100, 100, 32, 32, 2);
entity.addComponent<SpriteComponent>("assets/sprite.png");
```

### Step 3: Update

```cpp
void Game::update(float deltaTime) {
    manager.refresh();           // Remove inactive entities
    manager.update(deltaTime);   // Update all systems
}
```

### Step 4: (Optional) Manual Rendering

```cpp
void Game::render() {
    SDL_RenderClear(renderer);
    
    // RenderSystem handles drawing, or manually draw groups
    for (auto& entity : manager.getGroup(groupPlayers)) {
        entity->draw();
    }
    
    SDL_RenderPresent(renderer);
}
```

---

## Complete Examples

### Example 1: Simple Player Entity

```cpp
auto& player = manager.addEntity();

// Position and size
player.addComponent<TransformComponent>(400, 300, 16, 16, 4);

// Animation
auto& anim = player.addComponent<AnimatedSpriteComponent>();
anim.addTex("assets/walk-right.png", Animation(0, 4, 100));
anim.addTex("assets/walk-left.png", Animation(0, 4, 100));
anim.playTex("assets/walk-right.png");

// Collision
player.addComponent<ColliderComponent>("player");

// Custom behavior
player.addComponent<PlayerComponent>();

// Add to group
player.addGroup(Game::groupPlayers);
```

### Example 2: Enemy with AI

```cpp
auto& enemy = manager.addEntity();

// Transform
auto& transform = enemy.addComponent<TransformComponent>(600, 400, 32, 32, 2);
transform.velocity.x = -50.0f; // Move left

// Sprite
enemy.addComponent<AnimatedSpriteComponent>("assets/enemy.png", Animation(0, 2, 150));

// Collision
enemy.addComponent<ColliderComponent>("enemy");

// AI Script
enemy.addComponent<EnemyComponent>();

// Group
enemy.addGroup(Game::groupEnemies);
```

### Example 3: Physics Object

```cpp
auto& box = manager.addEntity();

// Transform
box.addComponent<TransformComponent>(300, 100, 32, 32, 1);

// Physics
auto& physics = box.addComponent<PhysicsComponent>(2.0f, true); // mass=2, gravity=true
physics.friction = 0.98f;
physics.bounciness = 0.5f;

// Visual
box.addComponent<SpriteComponent>("assets/box.png");

// Collision
box.addComponent<ColliderComponent>("box");
```

### Example 4: Audio-Triggered Entity

```cpp
auto& soundEntity = manager.addEntity();
soundEntity.addComponent<TransformComponent>(500, 500, 16, 16, 1);
soundEntity.addComponent<AudioComponent>("laser", false, false); // Don't autoplay

// In a script component, trigger the sound:
void onCollision() {
    entity->getComponent<AudioComponent>().play();
}
```

### Example 5: Full Scene Setup

```cpp
class GameScene : public Scene {
public:
    void onEnter() override {
        // Initialize systems
        Game::manager.addSystem<InputSystem>();
        Game::manager.addSystem<ScriptSystem>();
        Game::manager.addSystem<PhysicsSystem>();
        Game::manager.addSystem<MovementSystem>();
        Game::manager.addSystem<AnimationSystem>();
        Game::manager.addSystem<CollisionSystem>();
        Game::manager.addSystem<AudioSystem>();
        Game::manager.addSystem<RenderSystem>();
        
        // Load map
        Map::LoadMap("assets/level1.csv", 4, 4);
        
        // Create player
        auto& player = Game::manager.addEntity();
        player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
        player.addComponent<AnimatedSpriteComponent>("assets/player.png", Animation(0, 4, 100));
        player.addComponent<ColliderComponent>("player");
        player.addComponent<PhysicsComponent>(1.0f, true);
        player.addComponent<PlayerComponent>();
        player.addGroup(Game::groupPlayers);
        
        // Create enemies
        for (int i = 0; i < 5; i++) {
            auto& enemy = Game::manager.addEntity();
            enemy.addComponent<TransformComponent>(200 + i * 100, 200, 32, 32, 2);
            enemy.addComponent<AnimatedSpriteComponent>("assets/enemy.png", Animation(0, 2, 150));
            enemy.addComponent<ColliderComponent>("enemy");
            enemy.addComponent<EnemyComponent>();
            enemy.addGroup(Game::groupEnemies);
        }
    }
    
    void update(float deltaTime) override {
        Game::manager.refresh();
        Game::manager.update(deltaTime);
    }
    
    void draw() override {
        // Draw tiles
        for (auto& tile : Game::manager.getGroup(Game::groupMap)) {
            tile->draw();
        }
        
        // Draw players
        for (auto& player : Game::manager.getGroup(Game::groupPlayers)) {
            player->draw();
        }
        
        // Draw enemies
        for (auto& enemy : Game::manager.getGroup(Game::groupEnemies)) {
            enemy->draw();
        }
    }
};
```

---

## Best Practices

### 1. System Order
Always add systems in this order:
1. Input (reads input)
2. Scripts (game logic)
3. Physics (forces)
4. Movement (position updates)
5. Animation (visual updates)
6. Collision (detection)
7. Audio (sound)
8. Render (drawing - **must be last**)

### 2. Component Design
- Keep components **data-only** (no logic)
- Use small, focused components
- Combine components for complex behavior

### 3. Entity Creation
- Create entities during scene setup (`onEnter()`)
- Use groups for efficient querying
- Call `manager.refresh()` regularly to clean up

### 4. Performance
- Use entity groups instead of checking all entities
- Remove inactive entities with `entity.destroy()`
- Avoid creating/destroying entities every frame

### 5. Script Components
- Put custom game logic in script components
- Access other components via `entity->getComponent<T>()`
- Use InputManager through `Game::inputManager`

---

## API Reference

### Manager API

```cpp
// Entity Management
Entity& addEntity()
void refresh()
std::vector<Entity*>& getGroup(Group group)
std::vector<std::unique_ptr<Entity>>& getEntities()

// System Management
template <typename T, typename... TArgs>
T& addSystem(TArgs&&... mArgs)

template <typename T>
T& getSystem()

// Update
void update(float deltaTime)
void draw()
```

### Entity API

```cpp
// Components
template <typename T, typename... TArgs>
T& addComponent(TArgs&&... mArgs)

template <typename T>
bool hasComponent() const

template <typename T>
T& getComponent() const

// Groups
void addGroup(Group group)
void delGroup(Group group)
bool hasGroup(Group group) const

// Lifecycle
void destroy()
bool isActive() const
void update(float deltaTime)
void draw()
```

### Component API

```cpp
Entity* entity;              // Reference to owner

virtual void init()          // Called when component is added
virtual void update(float deltaTime)  // Called each frame
virtual void draw()          // Called during rendering
```

### System API

```cpp
virtual void update(Manager& manager, float deltaTime) = 0
virtual void init()
```

---

## Troubleshooting

### Entity not rendering?
- Check if RenderSystem is added
- Ensure entity has TransformComponent + visual component
- Verify entity is active: `entity.isActive()`

### Collision not working?
- Both entities need ColliderComponent
- CollisionSystem must be added
- Check if entities are active

### Physics not applying?
- Entity needs PhysicsComponent + TransformComponent
- PhysicsSystem must be added before MovementSystem
- Set `useGravity = true` for gravity

### Animation not playing?
- Check AnimationSystem is added
- Verify animation was added with `addTex()`
- Call `playTex()` to start animation

---

## Summary

Your ECS system is **fully complete** with:

✅ **8 Systems**: Input, Script, Physics, Movement, Animation, Collision, Audio, Render  
✅ **10+ Components**: Transform, Sprite, Animation, Collider, Physics, Audio, etc.  
✅ **Entity Management**: Creation, groups, lifecycle  
✅ **Flexible Architecture**: Easy to extend with new components/systems

### Next Steps

1. Add systems to your scenes
2. Create entities with components
3. Write custom script components for game logic
4. Use entity groups for efficient processing

Happy coding! 🎮

