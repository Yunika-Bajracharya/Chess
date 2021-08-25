#include "../headers/Test.h"
#include <chrono>

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define NEW_FEN                                                                \
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"

void Test::standardPerft() {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  for (int i = 0; i < 5; i++) {
    std::cout << "Depth: " << i + 1 << std::endl;
    std::cout << "Number of Positions: ";

    auto t1 = high_resolution_clock::now();
    generateAllMoves(i, false);
    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << "Time taken: " << ms_int.count() << " ms" << std::endl;
  }
}

void Test::generateAllMoves(int depth, bool first) {
  /*
   * Generates all the moves upto a certain depth
   */

  BoardState state;
  std::vector<std::vector<Move>> allMoves;
  std::vector<Move> moves;

  state.players[0] = new Player("a", true);
  state.players[1] = new Player("b", false);

  Engine::handleFENString(STARTING_FEN, state);
  std::cout << getNumberOfMoves(depth, state, first) << std::endl;
}

int Test::getNumberOfMoves(int depth, const BoardState &state, bool first) {
  /*
   * Recursively generates all the moves
   */
  std::vector<std::vector<Move>> allMoves;
  std::vector<Move> moves;
  Engine::generateAllMoves(state, allMoves);

  // int loops = 0;
  int count = 0;
  for (std::vector<Move> &moveList : allMoves) {
    for (Move &move : moveList) {
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
        int num = 0;
        /*
        if (loops == 2) {
          std::cout << "---" << std::endl;
          num = getNumberOfMoves(depth - 1, s, true);
          std::cout << "---" << std::endl;
        } else {
          num = getNumberOfMoves(depth - 1, s, false);
        }
        */
        num = getNumberOfMoves(depth - 1, s, false);

        count += num;
        if (first) {
          move.display();
          std::cout << num << std::endl;
        }
      }
      // loops++;
    }
  }

  return count;
}
