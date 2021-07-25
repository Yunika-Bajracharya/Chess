#include "../headers/Rook.h"

Rook::Rook(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 4;
}

Rook::~Rook() {}

void Rook::generateLegalMoves() {}
