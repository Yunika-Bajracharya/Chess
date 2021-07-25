#include "Piece.h"

class Rook : public Piece {
public:
  Rook(Coordinate pos, bool isColorWhite);
  ~Rook() override;

  void generateLegalMoves() override;

private:
};
