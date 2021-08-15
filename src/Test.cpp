#include "../headers/Test.h"

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
//#define NEW_FEN "rnbqkbnr/ppp1pppp/8/3N4/8/8/PPPPPPPP/R1BQKBNR b KQkq - 0 1"
#define DEPTH 0

void Test::generateAllMoves(int depth, bool first) {
  BoardState state;
  std::vector<std::vector<Move>> allMoves;
  std::vector<Move> moves;

  state.players[0] = new Player("a", true);
  state.players[1] = new Player("b", false);

  Engine::handleFENString(NEW_FEN, state);
  std::cout << getNumberOfMoves(depth, state, first) << std::endl;
}

int Test::getNumberOfMoves(int depth, const BoardState &state, bool first) {
  std::vector<std::vector<Move>> allMoves;
  std::vector<Move> moves;
  Engine::generateAllMoves(state, allMoves);

  int count = 0;
  for (std::vector<Move> moveList : allMoves) {
    for (Move move : moveList) {
      BoardState s = state;
      s.dragPieceLocation = move.startPos;
      s.dragPieceId = s.getID(move.startPos);
      Engine::placePiece(move, s);
      if (depth == 0) {
        count++;
        if (first) {
          move.display();
          std::cout << 1 << std::endl;
        }
      } else {
        int num = getNumberOfMoves(depth - 1, s, false);
        count += num;
        if (first) {
          move.display();
          std::cout << num << std::endl;
        }
      }
    }
  }

  return count;
}
