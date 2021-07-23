#pragma once
#include "Piece.h"
#include "Structures.h"

class Gameboard;
class Engine;
class Player {
public:
  Player(std::string playerName, bool isPlayerColorWhite);
  ~Player();

  bool isWhite();
  // void addPiece(Piece *p);
  friend class Gameboard;
  friend class Engine;

private:
  std::string Name;
  std::vector<Piece *> pieces;
  bool _isWhite;
};
