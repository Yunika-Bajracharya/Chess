#pragma once
#include "BoardState.h"
#include "Game.h"
#include "Player.h"

#include "Pawn.h"
#include "TextureManager.h"

class Gameboard {
public:
  Gameboard();
  ~Gameboard();

  void update();
  void render();

private:
  BoardState state;
  Coordinate boardStartPos;
  Player *players[2];
  SDL_Texture *pieceTextures;
};
