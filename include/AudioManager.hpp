#ifndef AUDIOMANAGER_HPP
#define AUDIOMANAGER_HPP

#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
#include <memory>
#include <unordered_map>

class AudioManager {
 public:
  AudioManager();
  ~AudioManager();

  bool loadAudio(const std::string& path, const std::string& key);
  void playAudio(const std::string& key, int loops = 0, int channel = -1);
  void setVolume(int volume, int channel = -1);
  void setPanning(int leftVolume, int rightVolume, int channel = -1);
  void applyEffect(const std::string& key, int effectType);

 private:
  std::unordered_map<std::string,
                     std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>>
      sounds;
};

#endif  // AUDIOMANAGER_HPP
