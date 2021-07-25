#include "Piece.h"

class Bishop : public Piece {
public:
  Bishop(Coordinate pos, bool isColorWhite);
  ~Bishop() override;

  void generateLegalMoves() override;

private:
};
