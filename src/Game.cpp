#include "../headers/Game.h"
#include "../headers/GameMenu.h"
#include "../headers/Gameboard.h"

SDL_Renderer *Game::renderer = nullptr;
Gameboard *board = nullptr;
GameMenu *menu = nullptr;

Game::Game() {}

Game::~Game() {}

void Game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {

  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

    if (window) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

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

    // board->init();
    stateMachine.AddState(new GameMenu(this));
    // stateMachine.AddState(new GameMenu());
    // stateMachine.ProcessStateChanges();
    // stateMachine.AddState(new Gameboard(), false);
    // board = new Gameboard();
    // board->init();

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
  // stateMachine.ProcessStateChanges();
  // board->update();
  stateMachine.GetActiveState()->update();
}
void Game::render() {

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // TODO
  // board->render();

  stateMachine.GetActiveState()->render();
  // board->render();

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

void Game::createGameBoard() { stateMachine.AddState(new Gameboard(), true); }
