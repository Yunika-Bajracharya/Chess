#pragma once
#include "Structures.h"

class Piece {
public:
  Piece(Coordinate pos, bool isColorWhite);
  ~Piece();

  virtual void generateLegalMoves() = 0;
  virtual int
  getTextureColumn() = 0; // The column refers to Gameboard.pieceTexture

  Coordinate getCoordinate();
  bool isWhite();

protected:
  int id;
  Coordinate position;
  bool isColorWhite;
};
