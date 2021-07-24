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
};

struct Move {
  Coordinate startPos;
  Coordinate endPos;
};
