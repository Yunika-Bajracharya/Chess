#include "Piece.h"

class Knight : public Piece {
public:
  Knight(Coordinate pos, bool isColorWhite);
  ~Knight() override;

  void generateLegalMoves() override;

private:
};
