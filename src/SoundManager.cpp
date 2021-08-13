#include "../headers/SoundManager.h"

void SoundManager::playSound(SoundManager::SoundType type) {
  switch (type) {
  case SoundManager::BlackMove: {

    Mix_Chunk *m = Mix_LoadWAV("assets/moveb.wav");
    Mix_PlayChannel(-1, m, 0);
    // Mix_FreeChunk(m);
    break;
  }
  case SoundManager::WhiteMove: {
    Mix_Chunk *m = Mix_LoadWAV("assets/movew.wav");
    Mix_PlayChannel(-1, m, 0);
    // Mix_FreeChunk(m);
    break;
  }
  }
}
