#pragma once
#include "Piece.h"
#include "Structures.h"

class Gameboard;
class Engine;
struct BoardState;
class Player {
public:
  Player(std::string playerName, bool isPlayerColorWhite);
  ~Player();

  bool isWhite();
  std::string getName();
  // void addPiece(Piece *p);
  friend class Gameboard;
  friend class Engine;
  friend struct BoardState;

private:
  std::string Name;
  std::vector<Piece *> pieces;
  bool _isWhite;

  int time;
};
