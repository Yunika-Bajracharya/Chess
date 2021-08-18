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
#include "Texture.h"

#include "GameState.h"

class Gameboard : public GameState {
public:
  Gameboard(std::string name1 = "Player 1", std::string name2 = "Player 2",
            int _startTimeInMinutes = 10);
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

  LastMove lastMove;
  Player *players[2];
  int playerTime[2];
  std::string PlayerNames[2];
  bool hasPlayedMove[2];
  int startTimeInMinutes;

  SDL_Rect resetButtonRect;
  Texture horizontalNotation[8];
  Texture verticalNotation[8];

  Texture playerNamesTexture[2]; // Stores the texture of player

  Texture resetButtonTexture;
  Texture checkTexture, checkMateTexture, outOfTimeTexture, matchDrawTexture;
  Texture numberTextures[10]; // Stores the textures for numbers 0-9
  Texture wonTexture;
  Texture colonTexture;

  Texture pieceTexture; // Stores texture of all pieces
};
