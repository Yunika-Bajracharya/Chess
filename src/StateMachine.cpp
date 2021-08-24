#include "../headers/StateMachine.h"

StateMachine::StateMachine() {}
StateMachine::~StateMachine() { clear(); }

void StateMachine::AddState(GameState *newState, bool isReplacing) {
  if (!this->_states.empty()) {
    if (isReplacing) {
      delete this->_states.top();
      this->_states.pop();
    }
  }
  this->_states.push((newState));
  this->_states.top()->init();
}

void StateMachine::clear() {
  size_t count = this->_states.size();
  for (size_t i = 0; i < count; i++) {
    delete this->_states.top();
    this->_states.pop();
  }
}

GameState *StateMachine::GetActiveState() { return this->_states.top(); }
