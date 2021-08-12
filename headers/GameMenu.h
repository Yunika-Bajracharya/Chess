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
  void loadImg();


private:
  SDL_Rect startButton,exitButton;
  Game *gameRef;
  SDL_Surface *start, *exit;
  SDL_Texture *startTexture, *exitTexture;
};
