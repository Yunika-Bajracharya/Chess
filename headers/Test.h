#include "Engine.h"
#include "Gameboard.h"
#define NEW_FEN                                                                \
  "1N2k2r/p1ppqpb1/bn2pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQk - 0 1"
#define DEPTH 0

class Test {
public:
  void generateAllMoves(int depth, bool first);

private:
  int getNumberOfMoves(int depth, const BoardState &state, bool first);
};
