#include "../headers/StateMachine.h"

StateMachine::StateMachine() {}
StateMachine::~StateMachine() { clear(); }

void StateMachine::AddState(GameState *newState, bool isReplacing) {
  this->_newState = newState;
  if (!this->_states.empty()) {
    if (isReplacing) {
      delete this->_states.top();
      this->_states.pop();
    } else {
      this->_states.top()->pause();
    }
  }
  this->_states.push((this->_newState));
  this->_states.top()->init();
  this->_isAdding = false;
}

void StateMachine::RemoveState() { this->_isRemoving = true; }

void StateMachine::ProcessStateChanges() {
  if (_isRemoving && !this->_states.empty()) {
    delete this->_states.top();
    this->_states.pop();

    if (!this->_states.empty()) {
      this->_states.top()->resume();
    }

    this->_isRemoving = false;
  }
}

void StateMachine::clear() {
  size_t count = this->_states.size();
  for (size_t i = 0; i < count; i++) {
    delete this->_states.top();
    this->_states.pop();
  }
}

GameState *StateMachine::GetActiveState() { return this->_states.top(); }
