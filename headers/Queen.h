#include "Piece.h"

class Pawn;
class Queen : public SlidePiece {
public:
  Queen(Coordinate pos, bool isColorWhite);
  Piece *clone() override;

private:
};
