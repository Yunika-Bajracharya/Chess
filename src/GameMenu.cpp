#include "../headers/GameMenu.h"
#include "../headers/TextureManager.h"

bool buttonPress(int x, int y, const SDL_Rect &rect);

GameMenu::GameMenu(Game *_gameRef) : gameRef(_gameRef) {}
GameMenu::~GameMenu() {
  SDL_DestroyTexture(exitTexture);
  SDL_DestroyTexture(startTexture);
}

void GameMenu::init() {
  GameMenu::loadImg();
  // Initialize Texture and stuff
  SDL_QueryTexture(startTexture, NULL, NULL, &startButton.w, &startButton.h);
  startButton.x = WINDOW_WIDTH / 2 - startButton.w / 2;
  startButton.y = WINDOW_HEIGHT / 2 - startButton.h / 5 - startButton.h;
  SDL_QueryTexture(exitTexture, NULL, NULL, &exitButton.w, &exitButton.h);
  exitButton.x = WINDOW_WIDTH / 2 - exitButton.w / 2;
  exitButton.y = WINDOW_HEIGHT / 2;

  isNameOneTheFocus = false;
  names[0] = "Suban";
  names[1] = "Prabin";

  namesBoxRect[0].y = WINDOW_HEIGHT / 8;
  namesBoxRect[1].y = WINDOW_HEIGHT / 8 + WINDOW_HEIGHT / 12;
  namesBoxRect[0].x = namesBoxRect[1].x = WINDOW_WIDTH / 2;

  namesPromptTexture[0] = TextureManager::loadSentence("Player 1: ");
  namesPromptTexture[1] = TextureManager::loadSentence("Player 2: ");
  cursorTexture = TextureManager::loadSentence("|");
  displayCursor = true;
  count = 0;

  namesTexture[0] = TextureManager::loadSentence(names[0].c_str());
  namesTexture[1] = TextureManager::loadSentence(names[1].c_str());
}

void GameMenu::loadImg() {

  start = IMG_Load("./assets/start.png");
  if (!start) {
    printf("IMG_Load: %s\n", IMG_GetError());
    // handle error
  }
  exit = IMG_Load("./assets/exit.png");
  if (!exit) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }
  startTexture = SDL_CreateTextureFromSurface(Game::renderer, start);
  exitTexture = SDL_CreateTextureFromSurface(Game::renderer, exit);
  SDL_FreeSurface(start);
  SDL_FreeSurface(exit);
}

void GameMenu::handleInput(SDL_Event &event) {
  // Handle Input
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    int x = event.button.x;
    int y = event.button.y;

    if (buttonPress(x, y, startButton)) {
      std::cout << "We start the game here" << std::endl;
      gameRef->createGameBoard(names[0], names[1]);

    } else if (buttonPress(x, y, exitButton)) {
      std::cout << "Just Quit it" << std::endl;
      gameRef->exitGame();

    } else if (y > namesBoxRect[0].y &&
               y < namesBoxRect[0].y + namesBoxRect[0].h) {
      isNameOneTheFocus = false;

    } else if (y > namesBoxRect[1].y &&
               y < namesBoxRect[1].y + namesBoxRect[1].h) {
      isNameOneTheFocus = true;
    }

    break;
  }

  case SDL_TEXTINPUT: {
    if (!(SDL_GetModState() & KMOD_CTRL &&
          (event.text.text[0] == 'c' || event.text.text[0] == 'C' ||
           event.text.text[0] == 'v' || event.text.text[0] == 'V'))) {
      // Append character
      std::cout << event.text.text;
      names[isNameOneTheFocus] += event.text.text;
    }
    break;
  }

  case SDL_KEYDOWN: {
    // Handle backspace
    if (event.key.keysym.sym == SDLK_BACKSPACE &&
        names[isNameOneTheFocus].length() > 0) {
      // lop off character
      names[isNameOneTheFocus].pop_back();
    }
    break;
  }
  }
}

void GameMenu::render() {
  int promptCoordinateX = WINDOW_WIDTH / 3;
  SDL_Rect tempRect;
  // Renders the menu
  SDL_RenderCopy(Game::renderer, startTexture, NULL, &startButton);
  SDL_RenderCopy(Game::renderer, exitTexture, NULL, &exitButton);

  // Render name prompts
  for (int i = 0; i < 2; i++) {
    SDL_QueryTexture(namesPromptTexture[i], NULL, NULL, &tempRect.w,
                     &tempRect.h);
    tempRect.x = promptCoordinateX;
    tempRect.y = namesBoxRect[i].y;
    SDL_RenderCopy(Game::renderer, namesPromptTexture[i], NULL, &tempRect);

    SDL_QueryTexture(namesTexture[i], NULL, NULL, &namesBoxRect[i].w,
                     &namesBoxRect[i].h);
    SDL_RenderCopy(Game::renderer, namesTexture[i], NULL, &namesBoxRect[i]);
  }
  // Render the cursor
  if (displayCursor) {
    tempRect.y = namesBoxRect[isNameOneTheFocus].y;
    tempRect.x =
        namesBoxRect[isNameOneTheFocus].x + namesBoxRect[isNameOneTheFocus].w;
    SDL_QueryTexture(cursorTexture, NULL, NULL, &tempRect.w, &tempRect.h);
    SDL_RenderCopy(Game::renderer, cursorTexture, NULL, &tempRect);
  }
}

void GameMenu::update() {
  // Update information of the menu
  for (int i = 0; i < 2; i++) {
    if (count > 0) {
      SDL_DestroyTexture(namesTexture[i]);
    }
    if (names[i] != "") {
      // Render new text
      namesTexture[i] = TextureManager::loadSentence(names[i].c_str());
    }
    // Text is empty
    else {
      // Render space texture
      namesTexture[i] = TextureManager::loadSentence(" ");
    }
  }

  if (count % CURSOR_BLINK_RATE == 0) {
    displayCursor = !displayCursor;
  }
  count++;
}

void GameMenu::pause() {}
void GameMenu::resume() {}

bool buttonPress(int x, int y, const SDL_Rect &rect) {
  if (x > rect.x && y > rect.y && x < rect.x + rect.w && y < rect.y + rect.h) {
    return true;
  } else
    return false;
}
