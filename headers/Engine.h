#include "Gameboard.h"
#include "Structures.h"
#define ASCII_OFFSET 48

class Engine {
public:
  static void handleFENString(std::string fenString, BoardState &state,
                              Player *players[2]);
  /*
   * TODO
   * Handle piece selection and piece placement
   */

  static void handlePiecePlacement(Coordinate &destination, BoardState &state,
                                   const std::vector<Move> moves);

private:
  static void addPiece(Piece *piece, BoardState &state, Player *players[2]);
};
