#include "../headers/Engine.h"

void Engine::handleFENString(std::string fenString, BoardState &state,
                             Player *players[2]) {

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      state.board[i][j] = nullptr;
    }
  }

  // TODO
  addPiece(new Queen({2, 3}, true), state, players);
  addPiece(new King({4, 2}, true), state, players);
  addPiece(new Bishop({5, 3}, false), state, players);
  addPiece(new Rook({4, 6}, false), state, players);
  addPiece(new Pawn({6, 4}, true), state, players);
}

void Engine::addPiece(Piece *piece, BoardState &state, Player *players[2]) {
  Coordinate c = piece->getCoordinate();
  int index = piece->isWhite() ? 0 : 1;

  players[index]->pieces.push_back(piece);

  // Probably update the boardState
  state.board[c.i][c.j] = piece;
}

void Engine::handlePiecePlacement(Coordinate &destination, BoardState &state,
                                  const std::vector<Move> moves) {

  for (Move move : moves) {
    if (move.endPos == destination) {
      Coordinate startPos = state.dragPieceLocation;

      // We check if start position of piece is the same
      if (startPos != move.startPos) {
        continue;
      }

      Piece *movingPiece = state.getPiece(startPos);
      if (!movingPiece) {
        // We fked up somewhere
        continue;
      }

      /* We make the move
       */

      // Capture the piece if there is one
      if (state.getPiece(destination)) {
        state.getPiece(destination)->getCaptured();
      }

      // Move the piece to the new location
      state.board[startPos.i][startPos.j] = nullptr;
      state.board[destination.i][destination.j] = movingPiece;
      movingPiece->moveTo(destination);

      break;
    }
  }
}
