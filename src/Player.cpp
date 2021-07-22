#include "../headers/Player.h"

Player::Player(std::string playerName, bool isWhite) {
  this->_isWhite = isWhite;
  Name = playerName;
}

// void Player::addPiece(Piece *p) {this->pieces.push_back(p)};

bool Player::isWhite() { return _isWhite; }
