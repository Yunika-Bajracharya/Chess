#include "Piece.h"

class Pawn : public Piece {
public:
  Pawn(Coordinate pos, bool isColorWhite);
  ~Pawn();

  int getTextureRow();
  void move();

private:
  int textureRow;
};
