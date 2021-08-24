#include "../headers/SoundManager.h"

Mix_Chunk *SoundManager::bmove = nullptr;
Mix_Chunk *SoundManager::wmove = nullptr;
bool SoundManager::initialized = false;

void SoundManager::init() {
  /*
   * Initializes all the sound assets
   */
  wmove = Mix_LoadWAV("assets/movew.wav");
  bmove = Mix_LoadWAV("assets/moveb.wav");
}

void SoundManager::clean() {
  if (bmove) {
    Mix_FreeChunk(bmove);
  }
  if (wmove) {
    Mix_FreeChunk(wmove);
  }
}

void SoundManager::playSound(SoundManager::SoundType type) {
  /*
   * Plays a sound depeneding on the type
   */

  switch (type) {
  case SoundManager::BlackMove: {
    Mix_PlayChannel(-1, bmove, 0);
    break;
  }
  case SoundManager::WhiteMove: {
    Mix_PlayChannel(-1, wmove, 0);
    break;
  }
  }
}
