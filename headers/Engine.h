#pragma once
#include "Gameboard.h"
#include "Structures.h"
#define ASCII_OFFSET 48

class Engine {
public:
  static void handleFENString(std::string fenString, BoardState &state);
  /*
   * TODO
   * Handle piece selection and piece placement
   */

  static void handlePiecePlacement(Coordinate &destination, BoardState &state,
                                   const std::vector<Move> moves);

  // Returns true if the current player's king is under attack
  static bool canDirectAttackKing(const BoardState &state);

private:
  static void addPiece(Piece *piece, BoardState &state);
};
