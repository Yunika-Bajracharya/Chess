#include "../headers/TextureManager.h"
#include "../headers/Game.h"

SDL_Color TextureManager::Black = {0, 0, 0, 255};
SDL_Color TextureManager::Green = {0, 255, 0, 255};
SDL_Color TextureManager::Red = {255, 0, 0, 255};
SDL_Color TextureManager::darkGreen = {118, 150, 86};

SDL_Texture *TextureManager::loadTexture(const char *filename) {
  /*
   * Loads a image as texture
   */

  SDL_Surface *tempSurace = IMG_Load(filename);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurace);

  SDL_FreeSurface(tempSurace);
  return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect &src, SDL_Rect &dest) {
  /*
   * Draws texture
   */

  SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

SDL_Texture *TextureManager::loadSentence(const char *sentence, int fontSize,
                                          SDL_Color sentenceColor) {
  /*
   * Loads a sentence as texture
   */

  TTF_Font *Roboto = TTF_OpenFont("assets/Roboto.ttf", fontSize);
  // SDL_Color White = {255, 255, 255, 255};

  SDL_Surface *surfaceMessage =
      TTF_RenderText_Solid(Roboto, sentence, sentenceColor);
  SDL_Texture *Message =
      SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
  SDL_FreeSurface(surfaceMessage);
  TTF_CloseFont(Roboto);

  return Message;
}

// Copy and paste from stack overflow
void TextureManager::DrawFillCircle(SDL_Renderer *renderer, int x, int y,
                                    int radius) {

  int offsetx, offsety, d;
  int status;

  offsetx = 0;
  offsety = radius;
  d = radius - 1;
  status = 0;

  while (offsety >= offsetx) {

    status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                 x + offsety, y + offsetx);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                 x + offsetx, y + offsety);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                 x + offsetx, y - offsety);
    status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                 x + offsety, y - offsetx);

    if (status < 0) {
      break;
    }

    if (d >= 2 * offsetx) {
      d -= 2 * offsetx + 1;
      offsetx += 1;
    } else if (d < 2 * (radius - offsety)) {
      d += 2 * offsety - 1;
      offsety -= 1;
    } else {
      d += 2 * (offsety - offsetx - 1);
      offsety -= 1;
      offsetx += 1;
    }
  }
}
