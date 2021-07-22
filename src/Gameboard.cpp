#include "../headers/Gameboard.h"

Gameboard::Gameboard() {
  state = BoardState();

  // Some stuff here
  boardStartPos.j = WINDOW_WIDTH / 2 - 4 * BLOCK_WIDTH;
  boardStartPos.i = WINDOW_HEIGHT / 2 - 4 * BLOCK_WIDTH;

  // Creating Players
  players[0] = new Player("Suban", true);
  players[1] = new Player("Prabin", false);

  // Handle FEN string
  players[0]->pieces.push_back(new Pawn({2, 3}, true));

  // Load piece Textures
  pieceTextures = TextureManager::loadTexture("assets/pieces.png");
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

  srcRect.h = srcRect.w = 200;
  // Render all white's pieces
  for (Piece *piece : players[0]->pieces) {
    Coordinate tempCoordinate = piece->getCoordinate();
    destRect.x = boardStartPos.j + tempCoordinate.j * BLOCK_WIDTH;
    destRect.y = boardStartPos.i + tempCoordinate.i * BLOCK_WIDTH;

    srcRect.y = (piece->isWhite()) ? 0 : srcRect.h;
    srcRect.x = piece->getTextureRow() * 200;

    SDL_RenderCopy(Game::renderer, pieceTextures, &srcRect, &destRect);
  }
}
