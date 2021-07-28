#include "../headers/Engine.h"

void Engine::handleFENString(std::string fenString, BoardState &state,
                             Player *players[2]) {
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
          addPiece(new King({rank, column}, true), state, players);
          break;
        case 'k':
          addPiece(new King({rank, column}, false), state, players);
          break;
        case 'Q':
          addPiece(new Queen({rank, column}, true), state, players);
          break;
        case 'q':
          addPiece(new Queen({rank, column}, false), state, players);
          break;
        case 'N':
          addPiece(new Knight({rank, column}, true), state, players);
          break;
        case 'n':
          addPiece(new Knight({rank, column}, false), state, players);
          break;
        case 'B':
          addPiece(new Bishop({rank, column}, true), state, players);
          break;
        case 'b':
          addPiece(new Bishop({rank, column}, false), state, players);
          break;
        case 'R':
          addPiece(new Rook({rank, column}, true), state, players);
          break;
        case 'r':
          addPiece(new Rook({rank, column}, false), state, players);
          break;
        case 'P':
          addPiece(new Pawn({rank, column}, true), state, players);
          break;
        case 'p':
          addPiece(new Pawn({rank, column}, false), state, players);
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

void Engine::addPiece(Piece *piece, BoardState &state, Player *players[2]) {
  Coordinate c = piece->getCoordinate();
  int index = piece->isWhite() ? 0 : 1;

  players[index]->pieces.push_back(piece);

  // Probably update the boardState
  state.board[c.i][c.j] = piece;
}

void Engine::handlePiecePlacement(Coordinate &destination, BoardState &state,
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
      break;
    }
  }
}
