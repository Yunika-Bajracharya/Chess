#include "../headers/Queen.h"

Queen::Queen(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 1;
}

Queen::~Queen() {}

void Queen::generateLegalMoves() {}
