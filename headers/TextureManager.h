#pragma once

#include "Game.h"

class TextureManager {
public:
  static SDL_Texture *loadTexture(const char *filename);
  static void Draw(SDL_Texture *tex, SDL_Rect &src, SDL_Rect &dest);

  static SDL_Texture *loadSentence(const char *sentence, int fontSize = 48);

  static void DrawFillCircle(SDL_Renderer *renderer, int x, int y, int radius);
};
