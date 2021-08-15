#include <SDL2/SDL_mixer.h>

class SoundManager {
public:
  enum SoundType { BlackMove, WhiteMove };

  static void init();
  static void clean();
  static void playSound(SoundType type);

  static Mix_Chunk *bmove, *wmove;
  static bool initialized;
};
