#include "../headers/Knight.h"
#include "../headers/BoardState.h"

Knight::Knight(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 3;
}

Knight::~Knight() {}

Piece *Knight::clone() { return new Knight(*this); }

int Knight::generateAllMoves(const BoardState &state,
                             std::vector<Move> &moves) {
  /*
   * Generates all the moves for the knight
   * Returns the number of moves
   */

  moves.clear();
  int count = 0;
  for (int i = 0; i < 8; i++) {
    Coordinate tempPos = position;
    tempPos += knightDirectionOffset[i];

    if (tempPos.isValidBoardIndex()) {
      if (canMoveTo(tempPos, state)) {
        Move m;
        m.startPos = position;
        m.endPos = tempPos;

        moves.push_back(m);
        count++;
      }
    }
  }
  return count;
}
