#pragma once
#include "Game.h"
#include "GameState.h"
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
  SDL_Texture *namesTexture[2];
  SDL_Rect namesBoxRect[2];
  SDL_Texture *namesPromptTexture[2];
  SDL_Texture *cursorTexture;

  bool isNameOneTheFocus;
  bool displayCursor;
  int count;

  SDL_Rect startButton, exitButton;
  Game *gameRef;
  SDL_Surface *start, *exit;
  SDL_Texture *startTexture, *exitTexture;
};
