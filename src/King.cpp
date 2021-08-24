#include "../headers/King.h"
#include "../headers/BoardState.h"

King::King(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 0; // as per its position in image
};

King::~King() {}

Piece *King::clone() { return new King(*this); }

int King::generateAllMoves(const BoardState &state, std::vector<Move> &moves) {
  /*
   * Generates all the movesfor the king
   * Returns the number of moves
   */

  for (int i = 0; i < 8; i++) {
    Coordinate tempPos = position;
    tempPos += slideDirectionOffset[i];

    if (tempPos.isValidBoardIndex()) {
      if (canMoveTo(tempPos, state)) {
        Move m;
        m.startPos = position;
        m.endPos = tempPos;

        moves.push_back(m);

        /*
         * We check if can castle is available
         * This is only then moving in the 3rd(index = 2) or 4th(index = 3)
         * direction is legal.
         */
        if (i == 2 || i == 3) {
          int castleCheckOffset = isColorWhite ? 0 : 2;
          if (i == 3 && state.CastleAvailability[0 + castleCheckOffset] &&
              state.isEmpty(tempPos)) {
            tempPos += slideDirectionOffset[i];
            if (state.isEmpty(tempPos)) {
              moves.push_back({position, tempPos});
            }
          } else if (i == 2 &&
                     state.CastleAvailability[1 + castleCheckOffset] &&
                     state.isEmpty(tempPos)) {
            tempPos += slideDirectionOffset[i];
            if (state.isEmpty(tempPos) &&
                state.isEmpty(tempPos + Coordinate{0, -1})) {
              moves.push_back({position, tempPos});
            }
          }
        }
      }
    }
  }
  return moves.size();
}
