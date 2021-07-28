#include "Piece.h"
#include "Structures.h"

struct BoardState {
  /*
   * Represents the state of the class BoardState
   */

  Piece *board[8][8]; // The 8 x8 Grid

  bool CastleAvailability[4]; // If we can castle
  bool enPassantAvailable;    // If we can enPassant
  Coordinate enPassant;       // Where can we enPassant

  bool isWhiteTurn; // True if it's white's turn

  int dragPieceId;              // Stores the id of the piece being dragged
  Coordinate dragPieceLocation; // Stores the locaiton of the grid

  Piece *getPiece(Coordinate location) { return board[location.i][location.j]; }
  bool isPieceWhite(Coordinate location) const {
    return board[location.i][location.j]->isWhite();
  }

  bool isEmpty(Coordinate location) const {
    if (board[location.i][location.j]) {
      return false;
    } else
      return true;
  }
};
