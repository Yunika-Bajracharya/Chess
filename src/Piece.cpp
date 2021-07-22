#include "../headers/Piece.h"

Piece::Piece(Coordinate pos, bool isColorWhite) : position(pos) {
  this->isColorWhite = isColorWhite;
}

Piece::~Piece() {}

Coordinate Piece::getCoordinate() { return position; }

bool Piece::isWhite() { return isColorWhite; }
