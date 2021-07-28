#include "../headers/Player.h"

Player::Player(std::string playerName, bool isWhite) {
  this->_isWhite = isWhite;
  Name = playerName;
  time = 10 * 60;
}

Player::~Player() {
  for (long unsigned int i = 0; i < pieces.size(); i++) {
    delete pieces[i];
  }
}

// void Player::addPiece(Piece *p) {this->pieces.push_back(p)};

bool Player::isWhite() { return _isWhite; }
