#include "../headers/StateMachine.h"

StateMachine::StateMachine() {}
StateMachine::~StateMachine() {}

void StateMachine::AddState(GameState *newState, bool isReplacing) {
  this->_isAdding = true;
  this->_isReplacing = isReplacing;

  this->_newState = std::move(newState);
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

  if (_isAdding) {
    if (!this->_states.empty()) {
      if (this->_isReplacing) {
        delete this->_states.top();
        this->_states.pop();
      } else {
        this->_states.top()->pause();
      }
    }
    this->_states.push(std::move(this->_newState));
    this->_states.top()->init();
    this->_isAdding = false;
  }
}

GameState *StateMachine::GetActiveState() { return this->_states.top(); }
