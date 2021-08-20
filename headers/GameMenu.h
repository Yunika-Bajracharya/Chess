#pragma once
#include "Game.h"
#include "GameState.h"
#include "Texture.h"
#define CURSOR_BLINK_RATE 20

class GameMenu : public GameState {
public:
  GameMenu(Game *gameRef);
  ~GameMenu() override;

  void init() override;

  void handleInput(SDL_Event &event) override;

  void render() override;
  void update() override;

  void pause() override;
  void resume() override;
  void loadImg();

private:
  std::string names[2];
  SDL_Rect namesBoxRect[2];
  Texture cursorTexture;
  Texture namesPromptTexture[2];
  Texture namesTexture[2];

  bool isNameOneTheFocus;
  bool displayCursor;
  int count;

  SDL_Rect startButton, exitButton, backdrop,square,squar;
  Game *gameRef;
  Texture startTexture, exitTexture, backdropTexture, squareTexture, squarTexture;
};
