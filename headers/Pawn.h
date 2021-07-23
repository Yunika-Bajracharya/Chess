#include "Piece.h"

class Pawn : public Piece {
public:
  Pawn(Coordinate pos, bool isColorWhite);
  ~Pawn() override;

  int getTextureColumn() override;
  void generateLegalMoves() override;

private:
  int textureColumn; // Refers to the column of BoardState.pieceTexture
};
