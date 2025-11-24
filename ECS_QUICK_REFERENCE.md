# 🎮 ECS Quick Reference Guide

## 📋 At a Glance

**Status**: ✅ Fully Complete & Tested  
**Systems**: 8 (All Implemented)  
**Components**: 10+ (All Working)  
**Compilation**: ✅ Passes  

---

## 🚀 Quick Start (3 Steps)

### 1️⃣ Add Systems (Once per scene)
```cpp
void Scene::onEnter() {
    Game::manager.addSystem<InputSystem>();
    Game::manager.addSystem<ScriptSystem>();
    Game::manager.addSystem<PhysicsSystem>();
    Game::manager.addSystem<MovementSystem>();
    Game::manager.addSystem<AnimationSystem>();
    Game::manager.addSystem<CollisionSystem>();
    Game::manager.addSystem<AudioSystem>();
    Game::manager.addSystem<RenderSystem>();  // ALWAYS LAST!
}
```

### 2️⃣ Create Entities
```cpp
// Simple entity
auto& entity = Game::manager.addEntity();
entity.addComponent<TransformComponent>(x, y, w, h, scale);
entity.addComponent<SpriteComponent>("assets/sprite.png");

// Animated entity
auto& player = Game::manager.addEntity();
player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
auto& anim = player.addComponent<AnimatedSpriteComponent>();
anim.addTex("assets/walk.png", Animation(0, 4, 100));
anim.playTex("assets/walk.png");
player.addComponent<PlayerComponent>(); // Custom script
```

### 3️⃣ Update Loop
```cpp
void Game::update(float deltaTime) {
    Game::manager.refresh();           // Clean up
    Game::manager.update(deltaTime);   // Update all systems
}
```

**That's it!** The systems handle everything else automatically.

---

## 📦 Available Components

| Component | Purpose | Example |
|-----------|---------|---------|
| `TransformComponent` | Position, size, velocity | `addComponent<TransformComponent>(x, y, w, h, scale)` |
| `SpriteComponent` | Static image | `addComponent<SpriteComponent>("path.png")` |
| `AnimatedSpriteComponent` | Animated sprite | `addComponent<AnimatedSpriteComponent>()` |
| `ColliderComponent` | Collision box | `addComponent<ColliderComponent>("player")` |
| `PhysicsComponent` | Gravity, friction | `addComponent<PhysicsComponent>(mass, useGravity)` |
| `AudioComponent` | Sound effects | `addComponent<AudioComponent>("sound", autoPlay)` |
| `ScriptComponent` | Custom behavior | `addComponent<YourScript>()` |
| `TileComponent` | Map tiles | `addComponent<TileComponent>(...)` |
| `ButtonComponent` | UI buttons | `addComponent<ButtonComponent>()` |
| `LabelComponent` | Text labels | `addComponent<LabelComponent>()` |

---

## 🔧 Available Systems

| System | What It Does | Processes |
|--------|--------------|-----------|
| `InputSystem` | Updates input state | Global InputManager |
| `ScriptSystem` | Runs custom logic | ScriptComponent |
| `PhysicsSystem` | Applies forces | PhysicsComponent + Transform |
| `MovementSystem` | Updates positions | TransformComponent |
| `AnimationSystem` | Updates frames | AnimatedSpriteComponent |
| `CollisionSystem` | Detects overlaps | ColliderComponent |
| `AudioSystem` | Plays sounds | AudioComponent |
| `RenderSystem` | Draws everything | Sprite/Animation/Tile |

---

## 💡 Common Patterns

### Player Entity
```cpp
auto& player = Game::manager.addEntity();
player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
player.addComponent<AnimatedSpriteComponent>("assets/player.png", Animation(0, 4, 100));
player.addComponent<ColliderComponent>("player");
player.addComponent<PlayerComponent>();
player.addGroup(Game::groupPlayers);
```

### Enemy Entity
```cpp
auto& enemy = Game::manager.addEntity();
enemy.addComponent<TransformComponent>(600, 400, 32, 32, 2);
enemy.addComponent<SpriteComponent>("assets/enemy.png");
enemy.addComponent<ColliderComponent>("enemy");
enemy.addComponent<EnemyComponent>();
enemy.addGroup(Game::groupEnemies);
```

### Physics Object
```cpp
auto& box = Game::manager.addEntity();
box.addComponent<TransformComponent>(300, 100, 32, 32, 1);
box.addComponent<PhysicsComponent>(2.0f, true); // Affected by gravity
box.addComponent<SpriteComponent>("assets/box.png");
box.addComponent<ColliderComponent>("box");
```

### Projectile
```cpp
auto& bullet = Game::manager.addEntity();
auto& transform = bullet.addComponent<TransformComponent>(x, y, 8, 8, 1);
transform.velocity.x = 500.0f; // Fast moving
bullet.addComponent<SpriteComponent>("assets/bullet.png");
bullet.addComponent<ColliderComponent>("projectile");
bullet.addComponent<AudioComponent>("shoot", true); // Sound on spawn
bullet.addGroup(Game::groupProjectiles);
```

---

## 🎯 Custom Script Component

```cpp
class MyScript : public ScriptComponent {
public:
    void init() override {
        // Called once when component is added
        std::cout << "MyScript initialized!" << std::endl;
    }
    
    void update(float deltaTime) override {
        // Called every frame
        auto& transform = entity->getComponent<TransformComponent>();
        
        // Example: Move on input
        if (Game::inputManager.isPressed("MoveRight")) {
            transform.velocity.x = 200.0f;
        } else {
            transform.velocity.x = 0.0f;
        }
    }
};

// Use it:
entity.addComponent<MyScript>();
```

---

## 🔍 Entity Query Patterns

### Check for Component
```cpp
if (entity->hasComponent<ColliderComponent>()) {
    auto& collider = entity->getComponent<ColliderComponent>();
    // Use collider
}
```

### Query by Group
```cpp
// Get all players
auto& players = Game::manager.getGroup(Game::groupPlayers);
for (auto* player : players) {
    if (player->isActive()) {
        // Process player
    }
}
```

### Iterate All Entities
```cpp
for (auto& entity : Game::manager.getEntities()) {
    if (!entity->isActive()) continue;
    
    if (entity->hasComponent<HealthComponent>()) {
        // Process entities with health
    }
}
```

---

## ⚙️ Entity Groups

```cpp
// Define in Game.hpp
enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupProjectiles,
    groupUI
};

// Add entity to group
entity.addGroup(Game::groupPlayers);

// Query group
auto& players = Game::manager.getGroup(Game::groupPlayers);
```

---

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| Entity not rendering | Add RenderSystem, ensure entity has Transform + visual component |
| Entity not moving | Add MovementSystem, set velocity on TransformComponent |
| No collision | Add CollisionSystem, both entities need ColliderComponent |
| Animation not playing | Add AnimationSystem, call `playTex()` on AnimatedSpriteComponent |
| Physics not working | Add PhysicsSystem (before MovementSystem), set `useGravity = true` |
| Input not working | Add InputSystem first, use `Game::inputManager.isPressed()` |

---

## 📚 Documentation Files

- **`ECS_USAGE_GUIDE.md`** - Complete guide with examples (START HERE!)
- **`SYSTEMS_README.md`** - Detailed system documentation
- **`ECS_COMPLETE_STATUS.md`** - Status and feature list
- **`SYSTEMS_USAGE.cpp`** - Code examples

---

## ✨ Key Features

✅ Template-based type-safe component access  
✅ Automatic system execution  
✅ Entity groups for efficient queries  
✅ Component lifecycle (init/update/draw)  
✅ Automatic memory management  
✅ Exception handling for missing components  
✅ SDL2 integration  
✅ Physics simulation  
✅ Collision detection  
✅ Animation system  
✅ Audio playback  

---

## 🎓 Remember

1. **Add RenderSystem LAST** - Always the final system
2. **Call refresh()** - Remove inactive entities regularly
3. **Use Groups** - More efficient than checking all entities
4. **Components = Data** - Keep logic in systems/scripts
5. **System Order Matters** - Input → Logic → Physics → Movement → Animation → Collision → Audio → Render

---

## 🎉 You're Ready!

Your ECS is **fully functional** and **production-ready**. Start building your game!

**Happy Coding!** 🚀

