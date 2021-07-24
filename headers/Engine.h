#include "Gameboard.h"
class Engine {
public:
  static void handleFENString(std::string fenString, BoardState &state,
                              Player *players[2]);
  /*
   * TODO
   * Handle piece selection and piece placement
   */

  static void handlePiecePlacement(Coordinate &destination, BoardState &state,
                                   std::vector<Move> moves);

private:
  static void addPiece(Piece *piece, BoardState &state, Player *players[2]);
};
