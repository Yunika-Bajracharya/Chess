#include "../headers/Engine.h"

void Engine::handleFENString(std::string fenString, BoardState &state) {
  // First we initalize them into nullptr
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      state.board[i][j] = nullptr;
    }
  }

  int index = 0;
  int column;

  for (int rank = 0; rank < 8; rank++) {
    column = 0;
    while (fenString[index] != '/' && fenString[index] != ' ') {

      if (isdigit(fenString[index])) {
        column += (fenString[index] - ASCII_OFFSET); // ASCII_OFFSET '0'
        index++;
      }

      else {

        switch (fenString[index]) {
        case 'K':
          addPiece(new King({rank, column}, true), state);
          break;
        case 'k':
          addPiece(new King({rank, column}, false), state);
          break;
        case 'Q':
          addPiece(new Queen({rank, column}, true), state);
          break;
        case 'q':
          addPiece(new Queen({rank, column}, false), state);
          break;
        case 'N':
          addPiece(new Knight({rank, column}, true), state);
          break;
        case 'n':
          addPiece(new Knight({rank, column}, false), state);
          break;
        case 'B':
          addPiece(new Bishop({rank, column}, true), state);
          break;
        case 'b':
          addPiece(new Bishop({rank, column}, false), state);
          break;
        case 'R':
          addPiece(new Rook({rank, column}, true), state);
          break;
        case 'r':
          addPiece(new Rook({rank, column}, false), state);
          break;
        case 'P':
          addPiece(new Pawn({rank, column}, true), state);
          break;
        case 'p':
          addPiece(new Pawn({rank, column}, false), state);
          break;
        default:
          break;
        }
        ++index;
        ++column;
      }
    }
    ++index;
  }

  // current player's turn
  state.isWhiteTurn = (fenString[index] == 'w') ? true : false;
  index += 2;
  for (int index = 0; index < 4; index++) {
    state.CastleAvailability[index] = false;
  }

  // castling
  if (fenString[index] == '-') {
    index += 2;
  } else {
    while (fenString[index] != ' ') {
      switch (fenString[index]) {
      case 'K':
        state.CastleAvailability[0] = true;
        break;
      case 'Q':
        state.CastleAvailability[1] = true;
        break;
      case 'q':
        state.CastleAvailability[3] = true;
        break;
      case 'k':
        state.CastleAvailability[2] = true;
        break;
      }
      ++index;
    }
    ++index;
  }

  // en-passant
  if (fenString[index] == '-') {
    index += 2;
    state.enPassantAvailable = false;
    state.enPassant = {0, 0};
  } else {
    int right = fenString[index] - 'a';
    ++index;
    int down = 8 - (fenString[index] - '0');
    state.enPassantAvailable = true;
    state.enPassant = {down, right};
    ++index;
  }

  // half move and full move left
}

void Engine::addPiece(Piece *piece, BoardState &state) {
  Coordinate c = piece->getCoordinate();
  int index = piece->isWhite() ? 0 : 1;

  state.players[index]->pieces.push_back(piece);

  // Probably update the boardState
  state.board[c.i][c.j] = piece;
}

bool Engine::handlePiecePlacement(Coordinate &destination, BoardState &state,
                                  const std::vector<Move> moves) {

  for (Move move : moves) {
    if (move.endPos == destination) {
      Coordinate startPos = state.dragPieceLocation;

      // We check if start position of piece is the same
      if (startPos != move.startPos) {
        continue;
      }

      Piece *movingPiece = state.getPiece(startPos);
      if (!movingPiece) {
        // We fked up somewhere
        continue;
      }

      /* We make the move
       */

      // If pawn made a enPassant
      if (movingPiece->getTextureColumn() == 5) {
        int distanceI = move.startPos.i - move.endPos.i;
        if (state.isEmpty(move.endPos) && state.enPassantAvailable &&
            move.endPos == state.enPassant) {
          // we know this has to be a enPassant
          Piece *pieceToEnPassant =
              state.getPiece(destination + Coordinate({distanceI, 0}));

          pieceToEnPassant->getCaptured();
          state.board[destination.i + distanceI][destination.j] = nullptr;
        }
      }

      // Capture the piece if there is one
      if (state.getPiece(destination)) {
        state.getPiece(destination)->getCaptured();
      }

      // Move the piece to the new location
      state.board[startPos.i][startPos.j] = nullptr;
      state.board[destination.i][destination.j] = movingPiece;
      movingPiece->moveTo(destination);

      // If the move was a castle, casteling
      // TODO Remeber to remove castleability
      if (movingPiece->getTextureColumn() == 0) {
        if (move.startPos.j - move.endPos.j == -2) {
          Coordinate rookPos = {move.endPos.i, move.endPos.j + 1};
          Piece *rook = state.getPiece(rookPos);
          rookPos = {move.endPos.i, move.endPos.j - 1};
          state.board[rookPos.i][rookPos.j] = rook;
          rook->moveTo(rookPos);
        } else if (move.startPos.j - move.endPos.j == 2) {
          Coordinate rookPos = {move.endPos.i, move.endPos.j - 2};
          Piece *rook = state.getPiece(rookPos);
          rookPos = {move.endPos.i, move.endPos.j + 1};
          state.board[rookPos.i][rookPos.j] = rook;
          rook->moveTo(rookPos);
        }
        // now no casle
        if (movingPiece->isWhite()) {
          state.CastleAvailability[0] = false;
          state.CastleAvailability[1] = false;
        } else {
          state.CastleAvailability[2] = false;
          state.CastleAvailability[3] = false;
        }
      }

      // If pawn moves 2 squares
      state.enPassantAvailable = false;
      if (movingPiece->getTextureColumn() == 5) {
        int distanceI = move.startPos.i - move.endPos.i;
        if (distanceI == 2 || distanceI == -2) {
          state.enPassantAvailable = true;
          state.enPassant = move.endPos + Coordinate({distanceI / 2, 0});
        }
      }
      // Change the turn
      state.isWhiteTurn = !state.isWhiteTurn;
      return true;
    }
  }
  return false;
}

int Engine::generateAllMoves(const BoardState &state,
                             std::vector<std::vector<Move>> &allMoves) {

  int count = 0;
  // Makesure all move is completely clearred
  for (std::vector<Move> movelist : allMoves) {
    movelist.clear();
  }
  allMoves.clear();

  int playerIndex = state.isWhiteTurn ? 0 : 1;
  std::vector<Move> moves;

  for (Piece *p : state.players[playerIndex]->pieces) {
    count += p->generateLegalMoves(state, moves);
    allMoves.push_back(moves);
    moves.clear();
  }

  return count;
}
void Engine::getMovelist(const Coordinate &c,
                         std::vector<std::vector<Move>> &allMovesSrc,
                         std::vector<Move> &movesDest) {

  movesDest.clear();

  for (std::vector<Move> movelist : allMovesSrc) {
    if (movelist.size() == 0)
      continue;
    if (movelist.front().startPos == c) {
      // We found our list
      for (Move move : movelist) {
        movesDest.push_back(move);
      }
      break;
    }
  }
}

bool Engine::canDirectAttackKing(const BoardState &state) {
  bool turn = state.isWhiteTurn;
  int index = turn ? 0 : 1;
  int enemy = !turn ? 0 : 1;

  Coordinate enemyKingLocation;

  // Find the king
  bool kingFound = false;
  for (Piece *p : state.players[enemy]->pieces) {
    if (p->getTextureColumn() == 0) {
      enemyKingLocation = p->getCoordinate();
      kingFound = true;
    }
  }
  if (!kingFound) {
    return false;
  }

  // Check moves for each of enemy piece
  std::vector<Move> moves;
  for (Piece *p : state.players[index]->pieces) {

    if (p->isCaptured()) {
      continue;
    }

    moves.clear();
    p->generateAllMoves(state, moves);

    for (Move move : moves) {
      if (move.endPos == enemyKingLocation) {
        return true;
      }
    }
    moves.clear();
  }
  return false;
}
