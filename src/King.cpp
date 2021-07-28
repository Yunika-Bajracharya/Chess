#include "../headers/King.h"

King::King(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 0;
};

King::~King() {}

void King::generateLegalMoves(const BoardState &state,
                              std::vector<Move> &moves) {
  for (int i = 0; i < 8; i++) {
    Coordinate tempPos = position;
    tempPos += slideDirectionOffset[i];

    if (tempPos.isValidBoardIndex()) {
      if (canMoveTo(tempPos, state)) {
        Move m;
        m.startPos = position;
        m.endPos = tempPos;

        moves.push_back(m);
      }
    }
  }
}
