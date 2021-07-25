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
    temp.j = i + c.j;
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
