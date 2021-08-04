#include <SDL2/SDL_mixer.h>

class SoundManager {
public:
  enum SoundType { BlackMove, WhiteMove };

  static void playSound(SoundType type);
};
