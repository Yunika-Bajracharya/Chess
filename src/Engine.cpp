#include "../headers/Engine.h"

void Engine::handleFENString(std::string fenString, BoardState &state,
                             Player *players[2]) {
  // TODO
  addPiece(new Pawn({2, 3}, true), players);
  addPiece(new King({4, 2}, true), players);
  addPiece(new Pawn({5, 3}, false), players);
}

void Engine::addPiece(Piece *piece, Player *players[2]) {
  int index = piece->isWhite() ? 0 : 1;
  players[index]->pieces.push_back(piece);
}
