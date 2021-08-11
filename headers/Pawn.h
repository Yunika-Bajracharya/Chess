#include "Piece.h"

class Queen;
class Bishop;
class Rook;
class Knight;

class Pawn : public Piece {
public:
  Pawn(Coordinate pos, bool isColorWhite);
  ~Pawn() override;
  Piece *clone() override;

  void moveTo(Coordinate destination) override;

  int generateAllMoves(const BoardState &state,
                       std::vector<Move> &moves) override;

  Piece *toQueen();
  Piece *toBishop();
  Piece *toKnight();
  Piece *toRook();

private:
  bool doubleMove;
};
