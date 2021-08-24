#pragma once
#include <SDL2/SDL.h>

class GameState {
public:
  virtual ~GameState() {}

  virtual void init() = 0;
  virtual void handleInput(SDL_Event &event) = 0;
  virtual void update() = 0;
  virtual void render() = 0;
};
