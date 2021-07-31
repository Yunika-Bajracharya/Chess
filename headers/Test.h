#include "Engine.h"
#include "Gameboard.h"

class Test {
public:
  void generateAllMoves(int depth, bool first);

private:
  int getNumberOfMoves(int depth, const BoardState &state, bool first);
};
