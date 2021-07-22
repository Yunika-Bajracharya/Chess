#include "../headers/Player.h"

Player::Player(std::string playerName, bool isPlayerColorWhite)
    : isWhite(isPlayerColorWhite) {
  Name = playerName;
}
