#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "StateMachine.h"

namespace GameInfo {
enum State {
  menu,    // Display menu
  board,   // Display Board
  gameover // Display Gameover
};
};

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define BLOCK_WIDTH 80
#define FPS 60

class Game {

public:
  Game();
  ~Game();

  void init(const char *title, int xpos, int ypos, int width, int height,
            bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();
  void exitGame();

  bool running();

  static SDL_Renderer *renderer;

  void createGameBoard(std::string name0, std::string name1,
                       int _startTimeInMinutes = 10, bool _useEngine = false);
  void goBackToGameMenu();

private:
  bool isRunning;
  SDL_Window *window;

  GameInfo::State state;
  StateMachine stateMachine;
  int count = 0;
};
