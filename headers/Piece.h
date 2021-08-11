#pragma once
#include "Structures.h"

struct BoardState;
class Engine;

class Piece {
public:
  Piece(Coordinate pos, bool isColorWhite);
  virtual ~Piece();
  virtual Piece *clone() = 0;

  int generateLegalMoves(const BoardState &state, std::vector<Move> &moves);

  virtual int generateAllMoves(const BoardState &state,
                               std::vector<Move> &moves) = 0;

  int getTextureColumn(); // The column refers to Gameboard.pieceTexture
  int getID();
  Coordinate getCoordinate();
  bool isWhite();

  virtual void
  moveTo(Coordinate destination); // Moves the piece to the location

  void getCaptured();
  bool isCaptured();

  bool canMoveTo(Coordinate c, const BoardState &state);

  static Coordinate slideDirectionOffset[8];
  static Coordinate knightDirectionOffset[8];

  friend class Engine;

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

  int generateAllMoves(const BoardState &state,
                       std::vector<Move> &moves) override;

protected:
  int loopStartIndex;
  int loopStopIndex;
};
