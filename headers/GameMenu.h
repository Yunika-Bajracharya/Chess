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

  void loadImg();
  enum TimeIndex { One = 0, Ten, Thirty };

private:
  std::string names[2];
  SDL_Rect namesBoxRect[2];
  Texture cursorTexture;
  Texture namesPromptTexture[2];
  Texture namesTexture[2];

  bool isNameOneTheFocus;
  bool displayCursor;
  int count;
  TimeIndex timeIndex;

  SDL_Rect singleButton, twoButton, exitButton, backdrop, square, squar, min,
      tenmin, thirtymin;
  Texture singleTexture, twoTexture, exitTexture, backdropTexture,
      squareTexture, squarTexture;
  Texture minTexture[2], tenminTexture[2], thirtyminTexture[2];

  Game *gameRef;
};
