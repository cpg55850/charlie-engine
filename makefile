# clear && make && ./game
# Define compiler and flags
# Specifies the C++ compiler.
CXX = g++
# Compiler flags, including the standard and include paths.
CXXFLAGS = -std=c++14 -I/opt/homebrew/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2 -I/opt/homebrew/Cellar/sdl2_mixer/2.8.0/include/SDL2 -I/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2 -D_THREAD_SAFE
# Linker flags, including library paths and libraries.
LDFLAGS = -L/opt/homebrew/lib -L/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib -L/opt/homebrew/Cellar/sdl2_mixer/2.8.0/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Source files
SRC = ./src/ECS/*.cpp ./src/*.cpp

# Default target
all: game

# Release build
game:
	$(CXX) $(CXXFLAGS) $(SRC) -o game $(LDFLAGS)

# Debug build
debug: CXXFLAGS += -g
debug: game

# Clean target
clean:
	rm -f game
	rm -rf game.dSYM

# Phony targets
# Declares phony targets to avoid conflicts with files named all, game, debug, or clean.
.PHONY: all game debug clean
