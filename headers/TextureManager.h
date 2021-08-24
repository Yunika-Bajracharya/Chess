#pragma once
#include <SDL2/SDL.h>

class TextureManager {
public:
  static SDL_Texture *loadTexture(const char *filename);
  static void Draw(SDL_Texture *tex, SDL_Rect &src, SDL_Rect &dest);

  static SDL_Texture *loadSentence(const char *sentence, int fontSize = 48,
                                   SDL_Color = Black);

  static void DrawFillCircle(SDL_Renderer *renderer, int x, int y, int radius);

<<<<<<< HEAD
  static SDL_Color Black, Green, Red;
=======
  static SDL_Color Black, Green, darkGreen;
>>>>>>> ddc9a6e9edb85a868dce7be0f00552da9148e1ce
};
