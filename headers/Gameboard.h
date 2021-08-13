#pragma once
#include "BoardState.h"
#include "Game.h"
#include "Player.h"

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

#include "SoundManager.h"
#include "TextureManager.h"

#include "GameState.h"

class Gameboard : public GameState {
public:
  Gameboard();
  ~Gameboard() override;

  void init() override; // Loads all the textures, creates players, etc
  void setBoard();
  void resetBoard();

  // Handle events
  void handleInput(SDL_Event &event) override;
  void handleMouseDown(SDL_Event &event);
  void handleMouseUp(SDL_Event &event);

  void update() override;
  void render() override;

  void pause() override;
  void resume() override;

private:
  BoardState state;
  Coordinate boardStartPos; // The top left edge of board
  std::vector<Move> moves;
  std::vector<std::vector<Move>> allMoves;
  Promotion::uiInfo promotionInfo;
  lastMoveInfo::State lastMoveState;

  Move lastMove;
  Player *players[2];
  int playerTime[2];

  SDL_Rect resetButtonRect;
  SDL_Texture *resetButtonTexture;
  SDL_Texture *playerNamesTexture[2]; // Stores the texture of name of players
  SDL_Texture *pieceTexture;          // Stores texture of all pieces
  SDL_Texture *checkTexture, *checkMateTexture, *outOfTimeTexture,
      *matchDrawTexture;
  SDL_Texture *wonTexture;
  SDL_Texture *numberTextures[10]; // Stores the textures for numbers 0-9
  SDL_Texture *horizontalNotation[8];
  SDL_Texture *verticalNotation[8];
  SDL_Texture *colonTexture;
};
