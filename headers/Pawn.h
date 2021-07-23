#include "Piece.h"

class Pawn : public Piece {
public:
  Pawn(Coordinate pos, bool isColorWhite);
  ~Pawn();

  int getTextureColumn();
  void generateLegalMoves();

private:
  int textureColumn; // Refers to the column of BoardState.pieceTexture
};
