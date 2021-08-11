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

lastMoveInfo Engine::handlePiecePlacement(Coordinate &destination,
                                          BoardState &state,
                                          const std::vector<Move> moves,
                                          Promotion::uiInfo &promotionInfo,
                                          Promotion::promotion promotionType) {

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

      // This check exists because
      // In piece.cpp there is a little bit of code
      // And i want that code to completely ignore this part
      if (!(promotionInfo.location.i == 99)) {
        if (!promotionInfo.promotion && movingPiece->getTextureColumn() == 5 &&
            move.endPos.isPromotionSquare()) {
          promotionInfo.promotion = true;
          promotionInfo.location = move.endPos;
          return {false, false};
        }

        if (promotionInfo.promotion && promotionType != move.promotion) {
          continue;
        }
      }

      /* We make the move
       */

      bool check = placePiece(move, state);
      return {true, check};
    }
  }
  return {false, false};
}
bool Engine::placePiece(Move move, BoardState &state) {
  Piece *movingPiece = state.getPiece(move.startPos);
  // Moves the piece with no checks

  // If pawn made a enPassant
  if (movingPiece->getTextureColumn() == 5) {
    int distanceI = move.startPos.i - move.endPos.i;
    if (state.isEmpty(move.endPos) && state.enPassantAvailable &&
        move.endPos == state.enPassant) {
      // we know this has to be a enPassant
      Piece *pieceToEnPassant =
          state.getPiece(move.endPos + Coordinate({distanceI, 0}));

      pieceToEnPassant->getCaptured();
      state.board[move.endPos.i + distanceI][move.endPos.j] = nullptr;
    }
  }

  // Capture the piece if there is one
  Piece *destinationPiece = state.getPiece(move.endPos);
  if (destinationPiece) {
    destinationPiece->getCaptured();

    if (destinationPiece->getTextureColumn() == 4) {
      int colorOffset = destinationPiece->isWhite() ? 0 : 2;
      int positionOffset = destinationPiece->getCoordinate().j == 0 ? 1 : 0;
      state.CastleAvailability[colorOffset + positionOffset] = false;
    }
  }

  // Move the piece to the new location
  state.board[move.startPos.i][move.startPos.j] = nullptr;
  state.board[move.endPos.i][move.endPos.j] = movingPiece;
  movingPiece->moveTo(move.endPos);

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
    // now no castle
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

  // If rook moved, now we cannot castle
  if (movingPiece->getTextureColumn() == 4) {
    int colorBuffer = movingPiece->isWhite() ? 0 : 2;
    if (state.CastleAvailability[0 + colorBuffer] ||
        state.CastleAvailability[1 + colorBuffer]) {
      if (move.startPos.j == 0) {
        state.CastleAvailability[1 + colorBuffer] = false;
      }
      if (move.startPos.j == 7) {
        state.CastleAvailability[0 + colorBuffer] = false;
      }
    }
  }

  // Weyy Pawn promotion
  if (move.endPos.isPromotionSquare() && movingPiece->getTextureColumn() == 5) {

    Piece *p = nullptr;
    switch (move.promotion) {
    case Promotion::Queen: {
      p = dynamic_cast<Pawn *>(movingPiece)->toQueen();
      break;
    }
    case Promotion::Rook: {
      p = dynamic_cast<Pawn *>(movingPiece)->toRook();
      break;
    }
    case Promotion::Bishop: {
      p = dynamic_cast<Pawn *>(movingPiece)->toBishop();
      break;
    }
    case Promotion::Knight: {
      p = dynamic_cast<Pawn *>(movingPiece)->toKnight();
      break;
    }
    default:
      p = dynamic_cast<Queen *>(movingPiece);
      break;
    }

    if (p) {
      state.board[move.endPos.i][move.endPos.j] = p;
      p->id = movingPiece->id;

      int playerIndex = state.isWhiteTurn ? 0 : 1;

      for (auto i = state.players[playerIndex]->pieces.begin();
           i != state.players[playerIndex]->pieces.end(); ++i) {
        if (*i == movingPiece) {
          state.players[playerIndex]->pieces.erase(i);
          break;
        }
      }
      state.players[playerIndex]->pieces.push_back(p);
      delete movingPiece;
    }
  }

  bool check = canDirectAttackKing(state);
  // Change the turn
  state.isWhiteTurn = !state.isWhiteTurn;

  return check;
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
    if (p->isCaptured())
      continue;

    count += p->generateLegalMoves(state, moves);
    // count += p->generateAllMoves(state, moves);
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
