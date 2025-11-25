cmake_minimum_required(VERSION 3.15)
project(GameEngine)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories
include_directories(
        include
        engine/include
        engine
        /opt/homebrew/include/SDL2
        /opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2
        /opt/homebrew/Cellar/sdl2_mixer/2.8.0/include/SDL2
        /opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2
)

# Link directories for SDL libraries
link_directories(/opt/homebrew/lib)

# Compiler definitions (like -D_THREAD_SAFE)
add_definitions(-D_THREAD_SAFE)

# Gather ALL source files (game + engine)
file(GLOB_RECURSE ENGINE_SOURCES engine/*.cpp)
file(GLOB_RECURSE GAME_SOURCES src/*.cpp)
set(SOURCES
        ${ENGINE_SOURCES}
        ${GAME_SOURCES}
)

# Create executable target
add_executable(game ${SOURCES})

# Link SDL libraries
target_link_libraries(game
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)

# Release/Debug flags
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")