#include "../headers/Gameboard.h"

Gameboard::Gameboard() {
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
  players[0]->pieces.push_back(new Pawn({2, 3}, true));
  players[1]->pieces.push_back(new Pawn({5, 3}, players[1]->isWhite()));

  // Load piece Textures
  pieceTexture = TextureManager::loadTexture("assets/pieces.png");
}

void Gameboard::render() {

  // Render board
  SDL_Rect destRect;
  SDL_Rect srcRect;

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

      srcRect.y = (piece->isWhite()) ? 0 : srcRect.h;
      srcRect.x = piece->getTextureRow() * 200;

      SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);
    }
  }

  // Render Player Names
  destRect.x = WINDOW_WIDTH / 2 + 5 * BLOCK_WIDTH;
  for (int i = 0; i < 2; i++) {
    destRect.y = (i == 0) ? 0.1 * WINDOW_HEIGHT : 0.8 * WINDOW_HEIGHT;
    SDL_QueryTexture(playerNamesTexture[i], NULL, NULL, &destRect.w,
                     &destRect.h);
    SDL_RenderCopy(Game::renderer, playerNamesTexture[i], NULL, &destRect);
  }
}
