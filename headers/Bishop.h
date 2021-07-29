#include "Piece.h"

class Bishop : public SlidePiece {
public:
  Bishop(Coordinate pos, bool isColorWhite);
  Piece *clone() override;

private:
};
