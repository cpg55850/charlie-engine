#include "AudioManager.hpp"

AudioManager::AudioManager() {
  // if (SDL_Init(SDL_INIT_AUDIO) < 0) {
  //   std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError()
  //             << std::endl;
  //   return;
  // }

  if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
    std::cerr << "Mix_Init failed! Mix_Error: " << Mix_GetError() << std::endl;
    return;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError()
              << std::endl;
    return;
  }

  atexit(Mix_CloseAudio);
}

AudioManager::~AudioManager() {
  sounds.clear();  // unique_ptr will automatically call Mix_FreeChunk

  Mix_CloseAudio();
  Mix_Quit();
  // SDL_Quit();
}

bool AudioManager::loadAudio(const std::string& path, const std::string& key) {
  Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
  if (!sound) {
    std::cerr << "Failed to load sound! Mix_Error: " << Mix_GetError()
              << std::endl;
    return false;
  }
  sounds.emplace(key, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(
                          sound, Mix_FreeChunk));
  return true;
}

void AudioManager::playAudio(const std::string& key, int loops, int channel) {
  auto it = sounds.find(key);
  if (it != sounds.end()) {
    Mix_PlayChannel(channel, it->second.get(), loops);
  } else {
    std::cerr << "Sound not found: " << key << std::endl;
  }
}

void AudioManager::setVolume(int volume, int channel) {
  Mix_Volume(channel, volume);
}

void AudioManager::setPanning(int leftVolume, int rightVolume, int channel) {
  Mix_SetPanning(channel, leftVolume, rightVolume);
}

void AudioManager::applyEffect(const std::string& key, int effectType) {
  // Placeholder: SDL_mixer does not support effects directly.
}
