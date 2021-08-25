#include "Engine.h"
#include "Gameboard.h"
#define DEPTH 4

/*
 * A class used for peft tests
 */
class Test {
public:
  void standardPerft();
  void generateAllMoves(int depth, bool first);

private:
  int getNumberOfMoves(int depth, const BoardState &state, bool first);
};
