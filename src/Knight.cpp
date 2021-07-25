#include "../headers/Knight.h"

Knight::Knight(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 3;
}

Knight::~Knight() {}

void Knight::generateLegalMoves() {}
