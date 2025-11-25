#pragma once
#include <bitset>
#include <cstddef>
#include <array>

using ComponentID = std::size_t;
using Group = std::size_t;
using EntityID = std::size_t; // Added for contiguous component storage mapping

inline ComponentID getNewComponentTypeID() {
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 64;  // Can be adjusted based on requirements
constexpr std::size_t maxGroups = 32;

using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<class Component*, maxComponents>;