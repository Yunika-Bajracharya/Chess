#include "Gameboard.h"
class Engine {
public:
  static void handleFENString(std::string fenString, BoardState &state,
                              Player *players[2]);

private:
  static void addPiece(Piece *piece, Player *players[2]);
};
