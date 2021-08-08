#include "../headers/Piece.h"
#include "../headers/BoardState.h"
#include "../headers/Engine.h"

Piece::Piece(Coordinate pos, bool isColorWhite) : position(pos) {
  static int pieceID = 1;
  this->isColorWhite = isColorWhite;

  captured = false;

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
    {1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {2, -1}, {-2, -1}, {-2, 1},
};

void Piece::moveTo(Coordinate destination) { this->position = destination; }

int Piece::generateLegalMoves(const BoardState &state,
                              std::vector<Move> &moves) {
  std::vector<Move> pseudoLegalMoves;
  generateAllMoves(state, pseudoLegalMoves);

  int moveCount = 0;
  for (Move move : pseudoLegalMoves) {
    // Because of shallow copy this does not work, do not use it yet
    BoardState newState = state; // Somehow make this a full copy
    newState.dragPieceLocation = move.startPos;
    newState.dragPieceId = state.getID(move.startPos);
    Promotion::uiInfo temp = {false, Coordinate({99, 99})};
    Engine::handlePiecePlacement(move.endPos, newState, pseudoLegalMoves, temp);

    /*
     * TODO if our king is in direct line of fire from opponent,
     * bad move we no add
     */
    if (!Engine::canDirectAttackKing(newState)) {
      moves.push_back(move);
      moveCount += 1;
    }
  }
  return moveCount;
  // Copy the state, play the moves if legal add to moves
}

void Piece::getCaptured() { captured = true; }
bool Piece::isCaptured() { return captured; }

bool Piece::canMoveTo(Coordinate c, const BoardState &state) {
  if (state.isEmpty(c)) {
    return true;
  } else {
    if (state.isPieceWhite(c) == isColorWhite) {
      return false;
    } else {
    }
    return true;
  }
}

/*
 *  For class slidePiece
 */

SlidePiece::SlidePiece(Coordinate pos, bool isColorWhite)
    : Piece(pos, isColorWhite) {}
SlidePiece::~SlidePiece() {}

int SlidePiece::generateAllMoves(const BoardState &state,
                                 std::vector<Move> &moves) {
  moves.clear();
  int count = 0;
  for (int i = loopStartIndex; i < loopStopIndex; i++) {
    Coordinate tempPos = position;
    tempPos += slideDirectionOffset[i];

    while (tempPos.isValidBoardIndex()) {

      bool captureTime = false;
      if (!state.isEmpty(tempPos)) {
        bool landingColor = state.isPieceWhite(tempPos);
        // If landing color is equal to the piece's color
        if (landingColor == isColorWhite) {
          break;
        } else {
          captureTime = true;
        }
      }

      Move m;
      m.startPos = position;
      m.endPos = tempPos;
      moves.push_back(m);

      tempPos += slideDirectionOffset[i];
      count++;

      if (captureTime) {
        break;
      }
    }
  }
  return count;
}
