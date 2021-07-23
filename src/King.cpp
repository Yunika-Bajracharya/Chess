#include "../headers/King.h"

King::King(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 0;
};

King::~King() {}
void King::generateLegalMoves() {}
