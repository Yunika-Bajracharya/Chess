#include "../headers/Texture.h"
#include "../headers/Game.h"

Texture::Texture() {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture() { Texture::clean(); }

bool Texture::loadFromFile(std::string path) {
  Texture::clean();
  mTexture = TextureManager::loadTexture(path.c_str());
  if (mTexture) {
    SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
    return true;
  } else {
    return false;
  }
}

bool Texture::loadSentence(std::string sentence, int fontSize,
                           SDL_Color sentenceColor) {
  Texture::clean();
  mTexture =
      TextureManager::loadSentence(sentence.c_str(), fontSize, sentenceColor);
  if (mTexture) {
    SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
    return true;
  } else {
    return false;
  }
}

void Texture::setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(mTexture, alpha); }

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::clean() {
  if (mTexture) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = mHeight = 0;
  }
}

void Texture::render(SDL_Rect *destRect, SDL_Rect *srcRect) {
  SDL_RenderCopy(Game::renderer, mTexture, srcRect, destRect);
}
void Texture::render(int x, int y) {
  SDL_Rect tempRect = {x, y, mWidth, mHeight};
  SDL_RenderCopy(Game::renderer, mTexture, NULL, &tempRect);
}

int Texture::getWidth() { return mWidth; }
int Texture::getHeight() { return mHeight; }
void Texture::queryTexture(int &width, int &height) {
  width = getWidth();
  height = getHeight();
}
