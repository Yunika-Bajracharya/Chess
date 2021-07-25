#include "Piece.h"

class Queen : public Piece {
public:
  Queen(Coordinate pos, bool isColorWhite);
  ~Queen() override;

  void generateLegalMoves() override;

private:
};
