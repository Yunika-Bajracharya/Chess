#include "../headers/Queen.h"

Queen::Queen(Coordinate pos, bool isColorWhite)
    : SlidePiece(pos, isColorWhite) {

  textureColumn = 1;
  loopStartIndex = 0;
  loopStopIndex = 8;
}

Piece *Queen::clone() { return new Queen(*this); }
