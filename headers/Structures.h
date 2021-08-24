#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct lastMoveInfo {
  enum State { None, Check, CheckMate, Draw, OutofTime, Resign };
  bool success;
  State state;
};

struct Coordinate {
  int i, j;

  bool isValidBoardIndex() {
    if (i >= 0 && i < 8 && j >= 0 && j < 8) {
      return true;
    } else
      return false;
  }
  bool isPromotionSquare() { return (i == 0 || i == 7) ? true : false; }

  void display() {
    std::cout << "Pos: (" << i << ", " << j << ")" << std::endl;
  }
  int SquareValue() {
    float value = 0;
    if (!isValidBoardIndex())
      return value;

    if (i >= 4) {
      value += 7 - i;
    } else {
      value += i;
    }
    if (j >= 4) {
      value += 7 - j;
    } else {
      value += j;
    }

    return value / 2;
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

namespace Promotion {
enum promotion { None, Queen, Rook, Knight, Bishop };
struct uiInfo {
  bool promotion;
  Coordinate location;
};
} // namespace Promotion

struct Move {
  Coordinate startPos;
  Coordinate endPos;
  Promotion::promotion promotion;

  Move(Coordinate s, Coordinate e, Promotion::promotion p = Promotion::None)
      : startPos(s), endPos(e) {
    promotion = p;
  }
  Move() {}

  char *toBoardNotation(Coordinate positon) {
    char *temp = new char(3);
    temp[0] = positon.j + 97;                         // The first letter
    temp[1] = ((positon.i + 8) - 2 * positon.i) + 48; // The number
    temp[2] = '\0';

    return temp;
  }
  void display() {
    char *s = toBoardNotation(startPos);
    char *e = toBoardNotation(endPos);
    std::cout << s << e << " ";

    delete s;
    delete e;
  }
};
struct LastMove : public Move {
  bool made;
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
