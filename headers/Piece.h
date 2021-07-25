#pragma once
#include "Structures.h"

class Piece {
public:
  Piece(Coordinate pos, bool isColorWhite);
  virtual ~Piece();

  virtual void generateLegalMoves() = 0;

  int getTextureColumn(); // The column refers to Gameboard.pieceTexture

  int getID();
  Coordinate getCoordinate();
  bool isWhite();

  static Coordinate slideDirection[8];
  static Coordinate knightDirection[8];

protected:
  int id;
  Coordinate position;
  bool isColorWhite;
  int textureColumn;
};
