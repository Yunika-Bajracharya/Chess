#include "../headers/GameMenu.h"
#include "../headers/TextureManager.h"
#include <map>

std::map<int, int> IndexToTime{{0, 1}, {1, 10}, {2, 30}};

bool buttonPress(int x, int y, const SDL_Rect &rect);

GameMenu::GameMenu(Game *_gameRef) : gameRef(_gameRef) {}
GameMenu::~GameMenu() {}

void GameMenu::init() {
  GameMenu::loadImg();
  // Initialize Texture and stuff
  singleTexture.queryTexture(singleButton.w, singleButton.h);
  singleButton.x = singleButton.w / 2 + singleButton.w / 5 + 50;
  singleButton.y = WINDOW_HEIGHT / 4;
  twoTexture.queryTexture(twoButton.w, twoButton.h);
  twoButton.x = singleButton.x;
  twoButton.y = WINDOW_HEIGHT / 4 + twoButton.h + twoButton.h / 2;
  exitTexture.queryTexture(exitButton.w, exitButton.h);
  exitButton.x = twoButton.w / 2 + exitButton.w / 2 + 60;
  exitButton.y = WINDOW_HEIGHT / 4 + twoButton.h + twoButton.h / 2 +
                 twoButton.h + twoButton.h / 2;
  backdrop.w = WINDOW_WIDTH;
  backdrop.h = WINDOW_HEIGHT;
  backdrop.x = backdrop.y = 0;
  squareTexture.queryTexture(square.w, square.h);
  square.x = WINDOW_WIDTH / 2 + square.w / 2 - 50;
  square.y = WINDOW_HEIGHT / 4;
  squarTexture.queryTexture(squar.w, squar.h);
  squar.x = WINDOW_WIDTH / 2 + squar.w / 2 - 50;
  squar.y = WINDOW_HEIGHT / 4 + twoButton.h + twoButton.h / 2;
  minTexture[0].queryTexture(min.w, min.h);
  min.x = square.x + 50;
  min.y = tenmin.y = thirtymin.y = exitButton.y;
  tenminTexture[0].queryTexture(tenmin.w, tenmin.h);
  tenmin.x = square.x + 150;
  thirtyminTexture[0].queryTexture(thirtymin.w, thirtymin.h);
  thirtymin.x = square.x + 250;

  isNameOneTheFocus = false;
  names[0] = "Player 1";
  names[1] = "Player 2";

  namesBoxRect[0].y = square.y + 30;
  namesBoxRect[1].y = squar.y + 30;
  namesBoxRect[0].x = namesBoxRect[1].x = square.x + 220;

  namesPromptTexture[0].loadSentence("Player 1 name: ", 30);
  namesPromptTexture[1].loadSentence("Player 2 name: ", 30);
  cursorTexture.loadSentence("|", 30);
  displayCursor = true;
  count = 0;

  namesTexture[0].loadSentence(names[0].c_str(), 30);
  namesTexture[1].loadSentence(names[1].c_str(), 30);
  timeIndex = GameMenu::Ten;
}

void GameMenu::loadImg() {
  singleTexture.loadFromFile("./assets/single player.png");
  twoTexture.loadFromFile("./assets/two players.png");
  exitTexture.loadFromFile("./assets/exit.png");
  backdropTexture.loadFromFile("./assets/backdrop.png");
  squareTexture.loadFromFile("./assets/square.png");
  squarTexture.loadFromFile("./assets/square.png");
  minTexture[0].loadFromFile("./assets/1 min.png");
  tenminTexture[0].loadFromFile("./assets/10min.png");
  thirtyminTexture[0].loadFromFile("./assets/30mins.png");

  minTexture[1].loadFromFile("./assets/pressed1 min.png");
  tenminTexture[1].loadFromFile("./assets/pressed10min.png");
  thirtyminTexture[1].loadFromFile("./assets/pressed30mins.png");
}

void GameMenu::handleInput(SDL_Event &event) {
  // Handle Input
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    int x = event.button.x;
    int y = event.button.y;

    if (buttonPress(x, y, twoButton)) {
      gameRef->createGameBoard(names[0], names[1], IndexToTime[timeIndex],
                               false);

    } else if (buttonPress(x, y, exitButton)) {
      gameRef->exitGame();

    } else if (buttonPress(x, y, square)) {
      isNameOneTheFocus = false;

    } else if (buttonPress(x, y, squar)) {
      isNameOneTheFocus = true;

    } else if (buttonPress(x, y, singleButton)) {
      gameRef->createGameBoard(names[0], names[1], IndexToTime[timeIndex],
                               true);
    } else if (buttonPress(x, y, min)) {
      timeIndex = GameMenu::One;
    } else if (buttonPress(x, y, tenmin)) {
      timeIndex = GameMenu::Ten;
    } else if (buttonPress(x, y, thirtymin)) {
      timeIndex = GameMenu::Thirty;
    }

    break;
  }

  case SDL_TEXTINPUT: {
    if (!(SDL_GetModState() & KMOD_CTRL &&
          (event.text.text[0] == 'c' || event.text.text[0] == 'C' ||
           event.text.text[0] == 'v' || event.text.text[0] == 'V'))) {
      // Append character
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
  int promptCoordinateX = square.x + 10;

  SDL_Rect tempRect;
  // Renders the menu
  backdropTexture.render(&backdrop);
  singleTexture.render(&singleButton);
  twoTexture.render(&twoButton);
  exitTexture.render(&exitButton);
  squareTexture.render(&square);
  squarTexture.render(&squar);

  if (timeIndex == GameMenu::One)
    minTexture[0].render(&min);
  else
    minTexture[1].render(&min);

  if (timeIndex == GameMenu::Ten)
    tenminTexture[0].render(&tenmin);
  else
    tenminTexture[1].render(&tenmin);

  if (timeIndex == GameMenu::Thirty)
    thirtyminTexture[0].render(&thirtymin);
  else
    thirtyminTexture[1].render(&thirtymin);

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

bool buttonPress(int x, int y, const SDL_Rect &rect) {
  if (x > rect.x && y > rect.y && x < rect.x + rect.w && y < rect.y + rect.h) {
    return true;
  } else
    return false;
}
