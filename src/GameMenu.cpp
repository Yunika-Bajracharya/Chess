#include "../headers/GameMenu.h"

GameMenu::GameMenu(Game *_gameRef) : gameRef(_gameRef) {}
GameMenu::~GameMenu() {}

void GameMenu::init() {
  // Initialize Texture and stuff
  buttonDest.h = 80;
  buttonDest.w = 200;
  buttonDest.x = 400;
  buttonDest.y = 400;
}

void GameMenu::handleInput(SDL_Event &event) {
  // Handle Input
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    int x = event.button.x;
    int y = event.button.y;

    if (x > buttonDest.x && y > buttonDest.y &&
        x < buttonDest.x + buttonDest.w && y < buttonDest.y + buttonDest.h) {

      std::cout << "We start the game here" << std::endl;
      gameRef->createGameBoard();
    }
  }
}

void GameMenu::render() {
  // Renders the menu
  SDL_SetRenderDrawColor(Game::renderer, 200, 200, 0, 255);
  SDL_RenderFillRect(Game::renderer, &buttonDest);
}

void GameMenu::update() {
  // Update information of the menu
}

void GameMenu::pause() {}
void GameMenu::resume() {}
