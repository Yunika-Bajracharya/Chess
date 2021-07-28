#include "../headers/Knight.h"
#include "../headers/BoardState.h"

Knight::Knight(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 3;
}

Knight::~Knight() {}

void Knight::generateLegalMoves(const BoardState &state,
                                std::vector<Move> &moves) {
  moves.clear();
  for (int i = 0; i < 8; i++) {
    Coordinate tempPos = position;
    tempPos += knightDirectionOffset[i];

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
