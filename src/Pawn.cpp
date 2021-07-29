#include "../headers/Pawn.h"
#include "../headers/BoardState.h"

Pawn::Pawn(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 5;
  doubleMove = true;
}

Pawn::~Pawn() {}

Piece *Pawn::clone() { return new Pawn(*this); }

void Pawn::generateAllMoves(const BoardState &state, std::vector<Move> &moves) {
  int direction = isColorWhite ? -1 : 1;

  // Straight moves
  Coordinate destPos = position + Coordinate({direction, 0});
  if (destPos.isValidBoardIndex()) {
    if (state.isEmpty(destPos)) {
      moves.push_back({position, destPos});

      if (doubleMove) {
        destPos += Coordinate({direction, 0});
        if (destPos.isValidBoardIndex())
          if (state.isEmpty(destPos)) {
            moves.push_back({position, destPos});
          }
      }
    }
  }

  // Diagonal Moves
  int diagonalDirection[2] = {-1, 1};
  for (int i = 0; i < 2; i++) {
    destPos = position + Coordinate({direction, diagonalDirection[i]});
    if (destPos.isValidBoardIndex()) {
      if (!state.isEmpty(destPos)) {
        if (state.isPieceWhite(destPos) != isColorWhite) {
          moves.push_back({position, destPos});
        }
      } else {
        if (state.enPassantAvailable && destPos == state.enPassant) {
          moves.push_back({position, destPos});
        }
      }
    }
  }
}

void Pawn::moveTo(Coordinate destination) {
  this->position = destination;
  doubleMove = false;
}
