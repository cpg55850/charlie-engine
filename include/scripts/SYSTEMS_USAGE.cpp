// SYSTEMS USAGE EXAMPLE
// This file demonstrates how to use the ECS Systems in your game

#include "Game.hpp"
#include "ECS/Systems.hpp"
#include "Components.hpp"

/*
 * SYSTEM OVERVIEW
 * ===============
 *
 * The ECS (Entity Component System) now includes the following systems:
 *
 * 1. MovementSystem     - Updates entity positions based on velocity
 * 2. RenderSystem       - Renders all visual components (sprites, tiles, etc.)
 * 3. AnimationSystem    - Updates sprite animations
 * 4. CollisionSystem    - Detects and handles collisions between entities
 * 5. ScriptSystem       - Executes custom script components
 *
 * Systems are updated automatically by the Manager during the game loop.
 */

// ============================================================================
// EXAMPLE 1: Adding Systems to the Manager (typically done during init)
// ============================================================================

void exampleInitializeSystems() {
    // Systems are added to the manager and will be called in order
    // The order matters - systems added first update first

    Game::manager.addSystem<ScriptSystem>();      // Custom logic first
    Game::manager.addSystem<MovementSystem>();    // Then movement
    Game::manager.addSystem<AnimationSystem>();   // Then animations
    Game::manager.addSystem<CollisionSystem>();   // Then collision detection
    Game::manager.addSystem<RenderSystem>();      // Finally render

    // Note: You typically want RenderSystem to be last so it renders
    // the final state after all updates
}

// ============================================================================
// EXAMPLE 2: Creating entities that work with systems
// ============================================================================

void exampleCreatePlayer() {
    auto& player = Game::manager.addEntity();

    // Add components that systems will process
    player.addComponent<TransformComponent>(400, 320, 16, 16, 4);
    player.addComponent<AnimatedSpriteComponent>("assets/walk-right.png", Animation(0, 2, 100));
    player.addComponent<ColliderComponent>("player");
    player.addComponent<PlayerComponent>();  // Custom script component

    // The systems will automatically process this entity:
    // - MovementSystem will update position based on velocity
    // - AnimationSystem will update the sprite animation
    // - CollisionSystem will check for collisions
    // - ScriptSystem will run PlayerComponent logic
    // - RenderSystem will draw the sprite
}

void exampleCreateEnemy() {
    auto& enemy = Game::manager.addEntity();

    enemy.addComponent<TransformComponent>(600, 400, 32, 32, 2);
    enemy.addComponent<SpriteComponent>("assets/enemy.png");
    enemy.addComponent<ColliderComponent>("enemy");
    // No script component = no custom logic, just renders and collides

    enemy.addGroup(Game::groupEnemies);
}

// ============================================================================
// EXAMPLE 3: System execution flow in the game loop
// ============================================================================

void exampleGameLoop() {
    // In your main game loop (typically in Game::update):

    float deltaTime = 0.016f; // ~60 FPS

    // 1. Refresh removes inactive entities
    Game::manager.refresh();

    // 2. Update all systems - they query entities and process components
    Game::manager.update(deltaTime);

    // The Manager automatically calls update on all systems in order:
    // -> ScriptSystem::update()      (custom game logic)
    // -> MovementSystem::update()    (position updates)
    // -> AnimationSystem::update()   (animation frames)
    // -> CollisionSystem::update()   (collision detection)
    // -> RenderSystem::update()      (rendering)
}

// ============================================================================
// EXAMPLE 4: Accessing a specific system
// ============================================================================

void exampleAccessSystem() {
    // You can get a reference to a specific system if needed
    auto& renderSystem = Game::manager.getSystem<RenderSystem>();

    // Now you can call system-specific methods if you added any
    // Most of the time, you won't need to access systems directly
}

// ============================================================================
// EXAMPLE 5: Component-based movement (works with MovementSystem)
// ============================================================================

void exampleMovement() {
    auto& entity = Game::manager.addEntity();
    auto& transform = entity.addComponent<TransformComponent>();

    // Set velocity - MovementSystem will automatically update position
    transform.velocity.x = 100.0f;  // Move 100 units/sec right
    transform.velocity.y = 0.0f;

    // MovementSystem handles: position += velocity * deltaTime
}

// ============================================================================
// EXAMPLE 6: Scene with Systems
// ============================================================================

class GameScene : public Scene {
public:
    void onEnter() override {
        // Initialize systems once per scene
        Game::manager.addSystem<MovementSystem>();
        Game::manager.addSystem<AnimationSystem>();
        Game::manager.addSystem<CollisionSystem>();
        Game::manager.addSystem<RenderSystem>();

        // Create game entities
        auto& player = Game::manager.addEntity();
        player.addComponent<TransformComponent>(400, 300, 16, 16, 4);
        player.addComponent<AnimatedSpriteComponent>("assets/player.png", Animation(0, 4, 100));
        player.addComponent<ColliderComponent>("player");
        player.addGroup(Game::groupPlayers);
    }

    void update(float deltaTime) override {
        // Systems are updated automatically by manager
        Game::manager.update(deltaTime);
    }

    void draw() override {
        // RenderSystem handles drawing, but you can also manually draw groups
        for (auto& entity : Game::manager.getGroup(Game::groupPlayers)) {
            entity->draw();
        }
    }
};

// ============================================================================
// SYSTEM PROCESSING DETAILS
// ============================================================================

/*
 * MovementSystem:
 * - Processes: Entities with TransformComponent
 * - Updates: position based on velocity * deltaTime
 *
 * AnimationSystem:
 * - Processes: Entities with AnimatedSpriteComponent
 * - Updates: Animation frames based on time
 *
 * CollisionSystem:
 * - Processes: Entities with ColliderComponent
 * - Detects: Overlapping colliders using AABB
 * - Note: Collision response is handled by script components
 *
 * RenderSystem:
 * - Processes: Entities with visual components
 * - Renders: TileComponent, SpriteComponent, AnimatedSpriteComponent
 * - Order: Tiles first, then sprites, then animated sprites
 *
 * ScriptSystem:
 * - Processes: Entities with ScriptComponent
 * - Executes: Custom game logic defined in script components
 */

// ============================================================================
// PERFORMANCE TIPS
// ============================================================================

/*
 * 1. Systems iterate through ALL entities - use entity groups for optimization
 * 2. Add components only when needed - systems skip entities without required components
 * 3. Remove inactive entities regularly using manager.refresh()
 * 4. Order systems efficiently - frequently updated systems first
 * 5. Use entity groups (groupPlayers, groupEnemies) for targeted processing
 */

