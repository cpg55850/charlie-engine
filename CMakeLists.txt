# cmake -B build -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
# cmake --build build
# ./bin/game

cmake_minimum_required(VERSION 3.15)
project(GameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Collect sources
file(GLOB_RECURSE ENGINE_SOURCES engine/*.cpp)
file(GLOB_RECURSE GAME_SOURCES src/*.cpp)

# Shared headers directory (create `include/Game.hpp`)
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(game
        ${ENGINE_SOURCES}
        ${GAME_SOURCES}
)

# Include paths (add new include directory)
target_include_directories(game
        PRIVATE
        ${PROJECT_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/engine
        ${CMAKE_SOURCE_DIR}/engine/SDLCore
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

# Function to find either static or dynamic SDL2 target
function(find_sdl2_package pkg_name)
    set(dynamic_target "${pkg_name}::${pkg_name}")
    set(static_target "${pkg_name}::${pkg_name}-static")

    if(TARGET ${dynamic_target})
        set(${pkg_name}_TARGET ${dynamic_target} PARENT_SCOPE)
    elseif(TARGET ${static_target})
        set(${pkg_name}_TARGET ${static_target} PARENT_SCOPE)
    else()
        find_package(${pkg_name} CONFIG REQUIRED)
        set(${pkg_name}_TARGET ${dynamic_target} PARENT_SCOPE)
    endif()
endfunction()

# Use it for all SDL packages
find_sdl2_package(SDL2)
find_sdl2_package(SDL2_image)
find_sdl2_package(SDL2_mixer)
find_sdl2_package(SDL2_ttf)

# Link to the automatically detected targets
target_link_libraries(game PRIVATE
        ${SDL2_TARGET}
        ${SDL2_image_TARGET}
        ${SDL2_mixer_TARGET}
        ${SDL2_ttf_TARGET}
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(game PRIVATE -O2)
else()
    target_compile_options(game PRIVATE -g -O0 -DDEBUG)
endif()

# Path to assets
set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DST "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/assets")

# Custom command to copy assets after build
add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SRC} ${ASSETS_DST}
        COMMENT "Copying assets folder to build directory"
)