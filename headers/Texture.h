#pragma once
#include "TextureManager.h"
#include <iostream>

/*
 * Interface for the SDL_Texture
 */

class Texture {
public:
  Texture();
  ~Texture();

  bool loadFromFile(std::string path);
  bool loadSentence(std::string sentence, int fontSize = 48,
                    SDL_Color sentenceColor = TextureManager::Black);

  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setAlpha(Uint8 alpha);
  int getWidth();
  int getHeight();
  void queryTexture(int &width, int &height);

  void render(SDL_Rect *destRect, SDL_Rect *srcRect = NULL);
  void render(int x, int y);
  void clean();

private:
  SDL_Texture *mTexture;
  int mWidth, mHeight;
};
