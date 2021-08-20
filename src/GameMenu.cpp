#include "../headers/GameMenu.h"
#include "../headers/TextureManager.h"

bool buttonPress(int x, int y, const SDL_Rect &rect);

GameMenu::GameMenu(Game *_gameRef) : gameRef(_gameRef) {}
GameMenu::~GameMenu() { std::cout << "GameMenu destroyed" << std::endl; }

void GameMenu::init() {
  GameMenu::loadImg();
  // Initialize Texture and stuff
  startTexture.queryTexture(startButton.w, startButton.h);
  startButton.x = WINDOW_WIDTH / 2 - startButton.w / 2;
  startButton.y = WINDOW_HEIGHT / 2 ;
  exitTexture.queryTexture(exitButton.w, exitButton.h);
  exitButton.x = WINDOW_WIDTH / 2 - exitButton.w / 2;
  exitButton.y = WINDOW_HEIGHT / 2 + startButton.h/5 +startButton.h;
  backdrop.w = WINDOW_WIDTH;
  backdrop.h = WINDOW_HEIGHT;
  backdrop.x = backdrop.y = 0;
  squareTexture.queryTexture(square.w, square.h);
  square.x= WINDOW_WIDTH / 2  - square.w/2;
  square.y= WINDOW_HEIGHT/2  -WINDOW_HEIGHT/3;
  squarTexture.queryTexture(squar.w, squar.h);
  squar.x= WINDOW_WIDTH / 2  - squar.w/2;
  squar.y= WINDOW_HEIGHT/2  -WINDOW_HEIGHT/3 + squar.h/5 + squar.h;

  isNameOneTheFocus = false;
  names[0] = "Player 1";
  names[1] = "Player 2";

  namesBoxRect[0].y = WINDOW_HEIGHT/2 -WINDOW_HEIGHT/3 +30 ;
  namesBoxRect[1].y = WINDOW_HEIGHT/3 +30;
  namesBoxRect[0].x = namesBoxRect[1].x = WINDOW_WIDTH/2 +10;
 
  namesPromptTexture[0].loadSentence("Player 1 name: ", 30);
  namesPromptTexture[1].loadSentence("Player 2 name: ", 30);
  cursorTexture.loadSentence("|", 30);
  displayCursor = true;
  count = 0;

  namesTexture[0].loadSentence(names[0].c_str(), 30);
  namesTexture[1].loadSentence(names[1].c_str(), 30);
}

void GameMenu::loadImg() {
  startTexture.loadFromFile("./assets/start.png");
  exitTexture.loadFromFile("./assets/exit.png");
  backdropTexture.loadFromFile("./assets/backdrop.jpg");
  squareTexture.loadFromFile("./assets/square.png");
  squarTexture.loadFromFile("./assets/square.png");
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
  int promptCoordinateX = WINDOW_WIDTH / 2 - WINDOW_WIDTH/7 -10;

  SDL_Rect tempRect;
  // Renders the menu
  backdropTexture.render(&backdrop);
  startTexture.render(&startButton);
  exitTexture.render(&exitButton);
  squareTexture.render(&square);
  squarTexture.render(&squar);

  // Render name prompts
  for (int i = 0; i < 2; i++) {
    tempRect.x = promptCoordinateX;
    tempRect.y = namesBoxRect[i].y;
    namesPromptTexture[i].render(tempRect.x, tempRect.y);
    namesTexture[i].render(namesBoxRect[i].x, namesBoxRect[i].y);
  }
  // Render the cursor
  if (displayCursor) {
    tempRect.y = namesBoxRect[isNameOneTheFocus].y;
    tempRect.x = namesBoxRect[isNameOneTheFocus].x +
                 namesTexture[isNameOneTheFocus].getWidth();
    cursorTexture.render(tempRect.x, tempRect.y);
  }
}

void GameMenu::update() {
  // Update information of the menu
  for (int i = 0; i < 2; i++) {
    if (count > 0) {
    }
    if (names[i] != "") {
      // Render new text
      namesTexture[i].loadSentence(names[i].c_str(), 30);
      namesBoxRect[i].h = namesTexture[i].getHeight();
      namesBoxRect[i].w = namesTexture[i].getWidth();
    }
    // Text is empty
    else {
      // Render space texture
      namesTexture[i].loadSentence(" ", 30);
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
