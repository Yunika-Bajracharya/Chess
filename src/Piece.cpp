#include "../headers/Piece.h"

Piece::Piece(Coordinate pos, bool isColorWhite) : position(pos) {
  static int pieceID = 1;
  this->isColorWhite = isColorWhite;

  id = pieceID;
  pieceID++;
}

Piece::~Piece() {}

Coordinate Piece::getCoordinate() { return position; }

bool Piece::isWhite() { return isColorWhite; }

int Piece::getTextureColumn() { return textureColumn; }

int Piece::getID() { return id; }
