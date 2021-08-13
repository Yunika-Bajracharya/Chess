#pragma once

#include <memory>
#include <stack>

#include "GameState.h"

class StateMachine {
public:
  StateMachine();
  ~StateMachine();

  void AddState(GameState *newState, bool isReplacing = true);

  void ProcessStateChanges();

  void RemoveState();

  GameState *GetActiveState();
  void clear();

private:
  std::stack<GameState *> _states;
  GameState *_newState;

  bool _isAdding;
  bool _isRemoving;
  bool _isReplacing;
};
