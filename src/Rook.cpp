#include "../headers/Rook.h"

Rook::Rook(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 4;
}

Rook::~Rook() {}

void Rook::generateLegalMoves() {}

void Rook::generateLegalMoves(const BoardState &state,
                              std::vector<Move> &moves) {

  moves.clear();
  for (int i = 0; i < 4; i++) {
    Coordinate tempPos = position;
    tempPos += slideDirectionOffset[i];

    while (tempPos.isValidBoardIndex()) {
      Move m;
      m.startPos = position;
      m.endPos = tempPos;
      moves.push_back(m);

      tempPos += slideDirectionOffset[i];
    }
  }
}
