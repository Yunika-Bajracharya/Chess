#include "Piece.h"

class Rook : public SlidePiece {
public:
  Rook(Coordinate pos, bool isColorWhite);
  Piece *clone() override;

private:
};
