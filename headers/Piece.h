#pragma once
#include "Structures.h"

struct BoardState;
class Piece {
public:
  Piece(Coordinate pos, bool isColorWhite);
  virtual ~Piece();

  virtual void generateLegalMoves() = 0;

  virtual void generateLegalMoves(const BoardState &state,
                                  std::vector<Move> &moves);

  int getTextureColumn(); // The column refers to Gameboard.pieceTexture

  void moveTo(Coordinate destination); // Moves the piece to the location

  void getCaptured();
  bool isCaptured();

  int getID();
  Coordinate getCoordinate();
  bool isWhite();

  static Coordinate slideDirectionOffset[8];
  static Coordinate knightDirectionOffset[8];

protected:
  int id;
  Coordinate position;
  bool isColorWhite;
  int textureColumn;
  bool captured;
};

class SlidePiece : public Piece {
public:
  SlidePiece(Coordinate pos, bool isColorWhite);
  ~SlidePiece();

  void generateLegalMoves();
  void generateLegalMoves(const BoardState &state, std::vector<Move> &moves);

protected:
  int loopStartIndex;
  int loopStopIndex;
};
