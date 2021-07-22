#pragma once
#include "Structures.h"

class Piece {
public:
  Piece(Coordinate pos, bool isColorWhite);
  ~Piece();

  virtual void generateLegalMoves() = 0;
  virtual int getTextureRow() = 0;

  Coordinate getCoordinate();
  bool isWhite();

protected:
  Coordinate position;
  bool isColorWhite;
};
