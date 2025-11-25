#pragma once

#include "../../engine/components/TransformComponent.hpp"
#include "../../engine/components/AnimatedSpriteComponent.hpp"
#include "../../engine/components/AnimationStateComponent.hpp"
#include "../../engine/components/ColliderComponent.hpp"
#include "../components/CombatComponent.hpp"
#include "../../engine/components/LifetimeComponent.hpp"
#include "../components/ScriptComponent.hpp"
#include "../../engine/components/TileComponent.hpp"
#include "../../engine/components/SpriteComponent.hpp"
#include "../../engine/components/ButtonComponent.hpp"
#include "../../engine/components/LabelComponent.hpp"
#include "../../engine/components/InputComponent.hpp"
#include "../components/DamageComponent.hpp"
#include "../components/FlashOnHitComponent.hpp"

// Forward declare new components from system headers
// They are defined in their respective system files
class PhysicsComponent;
class AudioComponent;
