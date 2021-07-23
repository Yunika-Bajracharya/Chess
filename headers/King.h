#include "Piece.h"

class King : public Piece {
public:
  King(Coordinate pos, bool isColorWhite);

  ~King();
  void generateLegalMoves() override;
};
