#include "../headers/TextureManager.h"

SDL_Texture *TextureManager::loadTexture(const char *filename) {
  SDL_Surface *tempSurace = IMG_Load(filename);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurace);

  SDL_FreeSurface(tempSurace);
  return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect &src, SDL_Rect &dest) {

  SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}
