#include "Piece.h"

class Rook : public Piece {
public:
  Rook(Coordinate pos, bool isColorWhite);
  ~Rook() override;

  void generateLegalMoves() override;
  void generateLegalMoves(const BoardState &state,
                          std::vector<Move> &moves) override;

private:
};
