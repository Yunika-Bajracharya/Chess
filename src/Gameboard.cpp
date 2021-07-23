#include "../headers/Gameboard.h"
#include "../headers/Engine.h"

Gameboard::Gameboard() {}

Gameboard::~Gameboard() {
  SDL_DestroyTexture(playerNamesTexture[0]);
  SDL_DestroyTexture(playerNamesTexture[1]);
  SDL_DestroyTexture(pieceTexture);

  delete players[0];
  delete players[1];
}

void Gameboard::init() {
  state = BoardState();

  // Some stuff here
  boardStartPos.j = WINDOW_WIDTH / 2 - 4 * BLOCK_WIDTH;
  boardStartPos.i = WINDOW_HEIGHT / 2 - 4 * BLOCK_WIDTH;

  // Creating Players
  players[0] = new Player("Suban", true);
  players[1] = new Player("Yunika", false);

  // Creating Player Name textures
  for (int i = 0; i < 2; i++) {
    playerNamesTexture[i] =
        TextureManager::loadSentence(players[i]->Name.c_str());
  }

  // Handle FEN string
  Engine::handleFENString("", state, players);

  // Load piece Textures
  pieceTexture = TextureManager::loadTexture("assets/pieces.png");
}

void Gameboard::handleMouseDown(SDL_Event &event) {
  int x = event.button.x - boardStartPos.j;
  int y = event.button.y - boardStartPos.i;

  Coordinate location = {x / BLOCK_WIDTH, y / BLOCK_WIDTH};

  if (event.button.button == SDL_BUTTON_LEFT) {
    if (location.isValidBoardIndex()) {
      location.display();
    }

  } else if (event.button.button == SDL_BUTTON_RIGHT) {
  }
}

void Gameboard::handleMouseUp(SDL_Event &event) {}

void Gameboard::update() {}

void Gameboard::render() {

  SDL_Rect destRect; // where to render
  SDL_Rect srcRect;  // from where we render

  // Render board
  destRect.h = destRect.w = BLOCK_WIDTH;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((i + j) % 2 == 0) {
        SDL_SetRenderDrawColor(Game::renderer, 238, 238, 210, 255);
      } else {
        SDL_SetRenderDrawColor(Game::renderer, 118, 150, 86, 255);
      }
      destRect.x = boardStartPos.j + j * BLOCK_WIDTH;
      destRect.y = boardStartPos.i + i * BLOCK_WIDTH;
      SDL_RenderFillRect(Game::renderer, &destRect);
    }
  }

  // Render all white's pieces
  srcRect.h = srcRect.w = 200;
  for (int i = 0; i < 2; i++) {
    for (Piece *piece : players[i]->pieces) {
      Coordinate tempCoordinate = piece->getCoordinate();
      destRect.x = boardStartPos.j + tempCoordinate.j * BLOCK_WIDTH;
      destRect.y = boardStartPos.i + tempCoordinate.i * BLOCK_WIDTH;

      // If white lower row, if black upper row
      srcRect.y = (piece->isWhite()) ? 0 : srcRect.h;
      srcRect.x = piece->getTextureColumn() * srcRect.h;

      SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);
    }
  }

  // Render Player Names
  destRect.x = WINDOW_WIDTH / 2 + 5 * BLOCK_WIDTH;
  for (int i = 0; i < 2; i++) {
    destRect.y = (i == 0) ? 0.1 * WINDOW_HEIGHT : 0.8 * WINDOW_HEIGHT;

    // Gets the size of font width and height
    SDL_QueryTexture(playerNamesTexture[i], NULL, NULL, &destRect.w,
                     &destRect.h);

    SDL_RenderCopy(Game::renderer, playerNamesTexture[i], NULL, &destRect);
  }
}

// TODO
void Gameboard::handleInput(SDL_Event &event) {
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    handleMouseDown(event);
    break;
  case SDL_MOUSEBUTTONUP:
    handleMouseUp(event);
    break;
  }
}
void Gameboard::pause() {}
void Gameboard::resume() {}