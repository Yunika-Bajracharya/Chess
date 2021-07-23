#include "../headers/Pawn.h"

Pawn::Pawn(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 5;
}

Pawn::~Pawn() {}

int Pawn::getTextureColumn() { return textureColumn; }

void Pawn::generateLegalMoves() {}
