#include "Piece.h"

class Pawn : public Piece {
public:
  Pawn(Coordinate pos, bool isColorWhite);
  ~Pawn();

  int getTextureRow();
  void generateLegalMoves();

private:
  int textureRow;
};
