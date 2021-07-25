#include "../headers/Bishop.h"

Bishop::Bishop(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 2;
}

Bishop::~Bishop() {}

void Bishop::generateLegalMoves() {}
