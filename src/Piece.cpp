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
  /*
   * Gets all the moves that can be done by the given piece
   * Then filters all the legal moves
   */

  std::vector<Move> pseudoLegalMoves;
  generateAllMoves(state, pseudoLegalMoves);

  int moveCount = 0;
  for (Move move : pseudoLegalMoves) {
    // Because of shallow copy this does not work, do not use it yet
    BoardState newState = state; // Somehow make this a full copy
    newState.dragPieceLocation = move.startPos;
    newState.dragPieceId = state.getID(move.startPos);
    Promotion::uiInfo temp = {false, Coordinate({99, 99})};

    Piece *p = newState.getPiece(move.startPos);
    /*
     * TODO Fix Casteling
     * If the king is in check (direct line of fire) or any square along the
     * path is, not castleable
     *
     * if the moving piece is a king, we check if it is under direct line of
     * fire, if it is, we disable all casteling moves
     *
     */
    if (p->getTextureColumn() == 0 && (move.startPos.j - move.endPos.j > 1 ||
                                       move.startPos.j - move.endPos.j < -1)) {
      newState.isWhiteTurn = !newState.isWhiteTurn;
      if (Engine::canDirectAttackKing(newState)) {
        continue;
      }
      newState.isWhiteTurn = !newState.isWhiteTurn;

      // Since the side movement moves are checked before casteling
      // We can see if the side movement moves are legal, if so, do casteling
      int directionJ = move.endPos.j - move.startPos.j;
      bool found = false;
      for (Move move : moves) {
        if (move.endPos.j - move.startPos.j == directionJ / 2)
          found = true;
      }
      if (!found)
        continue;
    }

    Engine::handlePiecePlacement(move.endPos, newState, pseudoLegalMoves, temp);

    /*
     * if our king is in direct line of fire from opponent,
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
  /*
   * Returs true if the piece can move to a square
   */

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
 *  To encapsulate all the features of a sliding piece (Rook/ Bishop)
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
