#include "Piece.h"

class Pawn : public Piece {
public:
  Pawn(Coordinate pos, bool isColorWhite);
  ~Pawn() override;

  void moveTo(Coordinate destination) override;

  void generateLegalMoves(const BoardState &state,
                          std::vector<Move> &moves) override;

private:
  bool doubleMove;
};
