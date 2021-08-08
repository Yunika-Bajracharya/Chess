#pragma once
#include "Piece.h"
#include "Player.h"
#include "Structures.h"

struct BoardState {
  /*
   * Represents the state of the class BoardState
   */

  Piece *board[8][8]; // The 8 x8 Grid

  bool CastleAvailability[4]; // If we can castle
  bool enPassantAvailable;    // If we can enPassant
  Coordinate enPassant;       // Where can we enPassant

  bool isWhiteTurn; // True if it's white's turn

  int dragPieceId;              // Stores the id of the piece being dragged
  Coordinate dragPieceLocation; // Stores the locaiton of the grid

  Player *players[2];

  Piece *getPiece(Coordinate location) { return board[location.i][location.j]; }
  Piece *getPiece(int id) {
    for (int i = 0; i < 2; i++) {
      for (Piece *p : players[i]->pieces) {
        if (p->getID() == id) {
          return p;
        }
      }
    }

    return nullptr;
  }

  bool isPieceWhite(Coordinate location) const {
    return board[location.i][location.j]->isWhite();
  }
  int getID(Coordinate location) const {
    return board[location.i][location.j]->getID();
  }

  bool isEmpty(Coordinate location) const {
    if (!location.isValidBoardIndex()) {
      return false;
    }
    if (board[location.i][location.j]) {
      return false;
    } else
      return true;
  }

  ~BoardState() {
    if (players[0] && players[1]) {
      delete players[0];
      delete players[1];
    }
  }

  // Copy Constructor
  BoardState(const BoardState &s)
      : enPassantAvailable(s.enPassantAvailable), enPassant(s.enPassant),
        isWhiteTurn(s.isWhiteTurn), dragPieceId(s.dragPieceId),
        dragPieceLocation(s.dragPieceLocation) {

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        board[i][j] = nullptr;
      }
    }

    for (int i = 0; i < 4; i++) {
      CastleAvailability[i] = s.CastleAvailability[i];
    }

    for (int i = 0; i < 2; i++) {
      players[i] = new Player(s.players[i]->getName(), s.players[i]->isWhite());
      for (Piece *p : s.players[i]->pieces) {
        Piece *newP = p->clone();
        players[i]->pieces.push_back(newP);
        Coordinate destination = newP->getCoordinate();
        if (!newP->isCaptured()) {
          board[destination.i][destination.j] = newP;
        }
      }
    }
  }
  BoardState() {
    players[0] = nullptr;
    players[1] = nullptr;
  }
};
