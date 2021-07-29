#include "Piece.h"

class King : public Piece {
public:
  King(Coordinate pos, bool isColorWhite);
  ~King() override;
  Piece *clone() override;

  void generateAllMoves(const BoardState &state,
                        std::vector<Move> &moves) override;

private:
};
