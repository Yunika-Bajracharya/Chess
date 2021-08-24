#include "../headers/Pawn.h"
#include "../headers/Bishop.h"
#include "../headers/BoardState.h"
#include "../headers/Knight.h"
#include "../headers/Queen.h"
#include "../headers/Rook.h"

Pawn::Pawn(Coordinate pos, bool isColorWhite) : Piece(pos, isColorWhite) {
  textureColumn = 5;
  if ((pos.i == 1 && !isColorWhite) || (pos.i == 6 && isColorWhite))
    doubleMove = true;
  else
    doubleMove = false;
}

Pawn::~Pawn() {}

// Uses the panw to create a new Queen/Rook/Knight/Bishop object
Piece *Pawn::toQueen() { return new Queen(position, isColorWhite); }
Piece *Pawn::toRook() { return new Rook(position, isColorWhite); }
Piece *Pawn::toKnight() { return new Knight(position, isColorWhite); }
Piece *Pawn::toBishop() { return new Bishop(position, isColorWhite); }

Piece *Pawn::clone() { return new Pawn(*this); }

int Pawn::generateAllMoves(const BoardState &state, std::vector<Move> &moves) {
  /*
   * Returns all the moves for a panw
   * returns the count
   */
  int direction = isColorWhite ? -1 : 1;

  int count = 0;
  // Straight moves
  Coordinate destPos = position + Coordinate({direction, 0});
  if (destPos.isValidBoardIndex()) {
    if (state.isEmpty(destPos)) {
      if (destPos.isPromotionSquare()) {
        moves.push_back({position, destPos, Promotion::Queen});
        moves.push_back({position, destPos, Promotion::Bishop});
        moves.push_back({position, destPos, Promotion::Rook});
        moves.push_back({position, destPos, Promotion::Knight});
        count += 4;
      } else {
        moves.push_back({position, destPos});
        count++;
      }

      if (doubleMove) {
        destPos += Coordinate({direction, 0});
        if (destPos.isValidBoardIndex())
          if (state.isEmpty(destPos)) {
            moves.push_back({position, destPos});
            count++;
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
          if (destPos.isPromotionSquare()) {
            moves.push_back({position, destPos, Promotion::Queen});
            moves.push_back({position, destPos, Promotion::Bishop});
            moves.push_back({position, destPos, Promotion::Rook});
            moves.push_back({position, destPos, Promotion::Knight});
            count += 4;
          } else {
            moves.push_back({position, destPos});
            count++;
          }
        }
      } else {
        if (state.enPassantAvailable && destPos == state.enPassant) {
          moves.push_back({position, destPos});
          count++;
        }
      }
    }
  }
  return count;
}

void Pawn::moveTo(Coordinate destination) {
  this->position = destination;
  doubleMove = false;
}
