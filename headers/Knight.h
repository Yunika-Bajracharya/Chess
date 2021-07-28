#include "Piece.h"

class Knight : public Piece {
public:
  Knight(Coordinate pos, bool isColorWhite);
  ~Knight() override;

  void generateLegalMoves(const BoardState &state,
                          std::vector<Move> &moves) override;

private:
};
