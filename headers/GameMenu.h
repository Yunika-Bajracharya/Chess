#pragma once
#include "Game.h"
#include "GameState.h"

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

private:
  SDL_Rect buttonDest;
  Game *gameRef;
};
