#include "../headers/Piece.h"

Piece::Piece(Coordinate pos, bool isColorWhite) : position(pos) {
  static int pieceID = 1;
  this->isColorWhite = isColorWhite;

  id = pieceID;
  pieceID++;
}

Piece::~Piece() {}

Coordinate Piece::getCoordinate() { return position; }

bool Piece::isWhite() { return isColorWhite; }

int Piece::getTextureColumn() { return textureColumn; }

int Piece::getID() { return id; }

Coordinate Piece::slideDirectionOffset[8] = {
    {1, 0}, {-1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

Coordinate Piece::knightDirectionOffset[8] = {
    {1, 2}, {-1, 2}, {2, -1}, {2, 1}, {-2, -1}, {-2, 1}, {1, -2}, {1, 2}};

void Piece::moveTo(Coordinate destination) { this->position = destination; }

void Piece::generateLegalMoves(const BoardState &state,
                               std::vector<Move> &moves) {}

/*
 *  For class slidePiece
 */

SlidePiece::SlidePiece(Coordinate pos, bool isColorWhite)
    : Piece(pos, isColorWhite) {}
SlidePiece::~SlidePiece() {}

void SlidePiece::generateLegalMoves() {}

void SlidePiece::generateLegalMoves(const BoardState &state,
                                    std::vector<Move> &moves) {
  moves.clear();
  for (int i = loopStartIndex; i < loopStopIndex; i++) {
    Coordinate tempPos = position;
    tempPos += slideDirectionOffset[i];

    while (tempPos.isValidBoardIndex()) {
      Move m;
      m.startPos = position;
      m.endPos = tempPos;
      moves.push_back(m);

      tempPos += slideDirectionOffset[i];
    }
  }
}
