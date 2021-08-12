#include "../headers/GameMenu.h"

GameMenu::GameMenu(Game *_gameRef) : gameRef(_gameRef) {}
GameMenu::~GameMenu() {}

void GameMenu::init() {
  GameMenu::loadImg();
  // Initialize Texture and stuff
  SDL_QueryTexture(startTexture,NULL,NULL,&startButton.w,&startButton.h);
  startButton.x = WINDOW_WIDTH/2-startButton.w/2;
  startButton.y = WINDOW_HEIGHT/2 - startButton.h/5 - startButton.h;
  SDL_QueryTexture(exitTexture,NULL,NULL,&exitButton.w,&exitButton.h);
  exitButton.x = WINDOW_WIDTH/2-exitButton.w/2;
  exitButton.y = WINDOW_HEIGHT/2 ;
}

void GameMenu::loadImg(){
    
    start=IMG_Load("./assets/start.png");
    if(!start) {
      printf("IMG_Load: %s\n", IMG_GetError());
      // handle error
    }
    exit=IMG_Load("./assets/exit.png");
    if(!exit) {
      printf("IMG_Load: %s\n", IMG_GetError());
    }
    startTexture = SDL_CreateTextureFromSurface(Game::renderer, start);
    exitTexture = SDL_CreateTextureFromSurface(Game::renderer, exit);
    SDL_FreeSurface(start);
    }
    

void GameMenu::handleInput(SDL_Event &event) {
  // Handle Input
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    int x = event.button.x;
    int y = event.button.y;

    if (x > startButton.x && y > startButton.y &&
        x < startButton.x + startButton.w && y < startButton.y + startButton.h) {

      std::cout << "We start the game here" << std::endl;
      gameRef->createGameBoard();
    }
    if (x > exitButton.x && y > exitButton.y &&
        x < exitButton.x + exitButton.w && y < exitButton.y + exitButton.h){
          std::cout<<"Just Quit it"<<std::endl;
          gameRef ->clean();
          }
  }
}

void GameMenu::render() {
 //SDL_SetRenderDrawColor(Game::renderer, 200, 200, 0, 255);
 //SDL_RenderFillRect(Game::renderer, &buttonDest);

  
  // Renders the menu
  SDL_RenderCopy(Game::renderer, startTexture, NULL, &startButton);
  SDL_RenderCopy(Game::renderer, exitTexture, NULL, &exitButton);
  //Draws the menu
  SDL_RenderPresent(Game::renderer);
}

void GameMenu::update() {
  // Update information of the menu
}

void GameMenu::pause() {}
void GameMenu::resume() {}
