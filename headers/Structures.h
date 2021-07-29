#pragma once
#include <iostream>
#include <string>
#include <vector>

struct Coordinate {
  int i, j;

  bool isValidBoardIndex() {
    if (i >= 0 && i < 8 && j >= 0 && j < 8) {
      return true;
    } else
      return false;
  }

  void display() {
    std::cout << "Pos: (" << i << ", " << j << ")" << std::endl;
  }

  bool operator==(Coordinate c) {
    if (c.i == i && c.j == j) {
      return true;
    } else {
      return false;
    }
  }

  bool operator!=(Coordinate c) {
    if (c.i != i || c.j != j) {
      return true;
    } else {
      return false;
    }
  }

  Coordinate operator+(Coordinate c) {
    Coordinate temp;
    temp.i = i + c.i;
    temp.j = j + c.j;
    return temp;
  }

  Coordinate operator+=(Coordinate c) {
    i += c.i;
    j += c.j;
    return *this;
  }
};

struct Move {
  Coordinate startPos;
  Coordinate endPos;
};

// Stores all the moves
struct AvailableMoves {
  std::vector<std::vector<Move>> allMoves;

  void getMoves(Coordinate startPos, std::vector<Move> &moves) {
    moves.clear();

    for (std::vector<Move> movelist : allMoves) {
      if (movelist.size() == 0)
        continue;
      if (movelist.front().startPos == startPos) {
        // We found our list
        for (Move move : movelist) {
          moves.push_back(move);
        }
        break;
      }
    }
  }
};
