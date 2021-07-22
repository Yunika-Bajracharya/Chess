#pragma once
#include "Piece.h"
#include "Structures.h"

class Gameboard;
class Player {
public:
  Player(std::string playerName, bool isPlayerColorWhite);
  ~Player();

  bool isWhite();
  // void addPiece(Piece *p);
  friend class Gameboard;

private:
  std::string Name;
  std::vector<Piece *> pieces;
  bool _isWhite;
};
