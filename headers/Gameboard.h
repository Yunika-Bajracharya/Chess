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

  void handleFENString(std::string fenString);

  // Handle events
  void handleMouseDown(SDL_Event &event);
  void handleMouseUp(SDL_Event &event);

  void update();
  void render();

private:
  BoardState state;
  Coordinate boardStartPos; // The top left edge of board

  Player *players[2];

  SDL_Texture *playerNamesTexture[2]; // Stores the texture of name of players
  SDL_Texture *pieceTexture;          // Stores texture of all pieces
};
