#include "../headers/Rook.h"

Rook::Rook(Coordinate pos, bool isColorWhite) : SlidePiece(pos, isColorWhite) {

  textureColumn = 4;
  loopStartIndex = 0;
  loopStopIndex = 4;
}

Piece *Rook::clone() { return new Rook(*this); }
