#include "../headers/Game.h"
#include "../headers/GameMenu.h"
#include "../headers/Gameboard.h"

SDL_Renderer *Game::renderer = nullptr;
Gameboard *board = nullptr;
GameMenu *menu = nullptr;

Game::Game() {}

Game::~Game() {
  clean();
  stateMachine.clear();
}

void Game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {

  // By default we have no flags
  int flags = 0;
  if (fullscreen) {
    // If fullscreen, flag -> make window fullscreen
    flags = SDL_WINDOW_FULLSCREEN;
  }

  // We Start SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    // Window Creation
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

    // Setting window color
    if (window) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    // Creating renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      std::cout << "renderer created" << std::endl;
    }
    // Opacity power
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
             Mix_GetError());
    }

    if (TTF_Init() == -1) {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
    }

    isRunning = true;
    SDL_StartTextInput();

    GameMenu *menu = new GameMenu(this);
    stateMachine.AddState(menu);
  } else {
    isRunning = false;
  }
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT: {
    isRunning = false;
    break;
  }

  default:
    stateMachine.GetActiveState()->handleInput(event);
    break;
  };
}

void Game::update() {
  count++;
  stateMachine.GetActiveState()->update();
}
void Game::render() {

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // TODO
  stateMachine.GetActiveState()->render();

  SDL_RenderPresent(renderer);
}
void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  std::cout << "Game cleared" << std::endl;
}

void Game::exitGame() { this->isRunning = false; }

bool Game::running() { return isRunning; }

void Game::createGameBoard(std::string name1, std::string name2,
                           int _startTimeInMinutes, bool useEngine) {
  if (name1.length() == 0) {
    name1 += "Player 1";
  }
  if (name2.length() == 0) {
    name2 += "Player 2";
  }
  stateMachine.AddState(
      new Gameboard(this, name1, name2, _startTimeInMinutes, useEngine), true);
}

void Game::goBackToGameMenu() {
  stateMachine.AddState(new GameMenu(this), true);
}
