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

SDL_Texture *TextureManager::loadSentence(const char *sentence) {
  TTF_Font *Roboto = TTF_OpenFont("Roboto.ttf", 48);
  // SDL_Color White = {255, 255, 255, 255};
  SDL_Color Black = {0, 0, 0, 255};

  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Roboto, sentence, Black);
  SDL_Texture *Message =
      SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);

  return Message;
}
