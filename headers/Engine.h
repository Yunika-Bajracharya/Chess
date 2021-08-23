#pragma once
#include "Gameboard.h"
#include "Structures.h"
#include <ctime>
#define ASCII_OFFSET 48

class Test;

class Engine {
public:
  static void handleFENString(std::string fenString, BoardState &state);
  /*
   * TODO
   * Handle piece selection and piece placement
   */

  static lastMoveInfo
  handlePiecePlacement(Coordinate &destination, BoardState &state,
                       const std::vector<Move> moves,
                       Promotion::uiInfo &promotionInfo,
                       Promotion::promotion promotionType = Promotion::None);

  // Returns true if the current player's king is under attack
  static bool canDirectAttackKing(const BoardState &state);

  // Generates moves for all the pieces of a given color
  static int generateAllMoves(const BoardState &state,
                              std::vector<std::vector<Move>> &allMoves);
  static void getMovelist(const Coordinate &c,
                          std::vector<std::vector<Move>> &allMovesSrc,
                          std::vector<Move> &movesDest);

  enum EngineDifficulty { None, Random };

  // static void setEngineDifficulty(EngineDifficulty _difficulty);
  static Move *generateAIMove(BoardState &state,
                              std::vector<std::vector<Move>> &allMoves);

  static bool placePiece(Move move, BoardState &state);
  friend class Test;

private:
  static void addPiece(Piece *piece, BoardState &state);
};
