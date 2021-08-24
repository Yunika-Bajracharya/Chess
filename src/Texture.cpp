#include "../headers/Texture.h"
#include "../headers/Game.h"

Texture::Texture() {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture() { Texture::clean(); }

bool Texture::loadFromFile(std::string path) {
  /*
   * Loads a texture from the given path
   */

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
  /*
   * Loads a texture from sentece
   */

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

// Sets the alpha for the texture
void Texture::setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(mTexture, alpha); }

// Sets the colormod for the texture
void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

// Destroys the texture
void Texture::clean() {
  if (mTexture) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = mHeight = 0;
  }
}

void Texture::render(SDL_Rect *destRect, SDL_Rect *srcRect) {
  /*
   * Renders a portion of the texture at a rect
   */
  SDL_RenderCopy(Game::renderer, mTexture, srcRect, destRect);
}
void Texture::render(int x, int y) {
  /*
   * Renders the entire texture starting from a point
   * Size dpeneds on size of the texture
   */

  SDL_Rect tempRect = {x, y, mWidth, mHeight};
  SDL_RenderCopy(Game::renderer, mTexture, NULL, &tempRect);
}

int Texture::getWidth() { return mWidth; }
int Texture::getHeight() { return mHeight; }
void Texture::queryTexture(int &width, int &height) {
  width = getWidth();
  height = getHeight();
}
