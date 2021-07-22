#include "Piece.h"
#include "Structures.h"

struct BoardState {
  Piece *board[8][8];

  bool CastleAvailability[4];
  bool enPassantAvailable;
  Coordinate enPassant;
};
