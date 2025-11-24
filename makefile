# clear && make && ./game
# Define compiler and flags
# Specifies the C++ compiler.
CXX = clang++
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = bin

# SDL include paths (Homebrew macOS) + project include dir
CXXFLAGS = -std=c++17 -I$(INCLUDE_DIR) -I/opt/homebrew/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include/SDL2 -I/opt/homebrew/Cellar/sdl2_mixer/2.8.0/include/SDL2 -I/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2 -D_THREAD_SAFE
# Linker flags, including library paths and libraries.
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# discover sources
SRC := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

.PHONY: all release debug clean run
all: release

release: CXXFLAGS += -O2
release: $(BIN_DIR)/game

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: $(BIN_DIR)/game

# link
$(BIN_DIR)/game: $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# compile to object, preserve subdirs in obj dir
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) game

run: $(BIN_DIR)/game
	./$(BIN_DIR)/game