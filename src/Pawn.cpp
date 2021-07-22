#include "../headers/Pawn.h"

Pawn::Pawn(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureRow = 5;
}

Pawn::~Pawn() {}

int Pawn::getTextureRow() { return textureRow; }

void Pawn::move() {}
