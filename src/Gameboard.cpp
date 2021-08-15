#include "../headers/Gameboard.h"
#include "../headers/Engine.h"
#include <SDL2/SDL_render.h>
#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace Promotion {
std::map<int, promotion> promotionMap{
    {0, None}, {1, Queen}, {2, Bishop}, {3, Knight}, {4, Rook}};
}

void clockTickToTime(int clock, int timeInfo[4]);

void makeMove(Coordinate location, BoardState &state,
              std::vector<std::vector<Move>> &allMoves,
              std::vector<Move> &moves, lastMoveInfo::State &lastMoveState,
              LastMove &lastMove, Promotion::uiInfo &promotionInfo,
              int promotionID = 0);

Gameboard::Gameboard(std::string name1, std::string name2) {
  PlayerNames[0] = name1;
  PlayerNames[1] = name2;
}

Gameboard::~Gameboard() {
  std::cout << "Gameboard Destroyed" << std::endl;
  SDL_DestroyTexture(playerNamesTexture[0]);
  SDL_DestroyTexture(playerNamesTexture[1]);
  SDL_DestroyTexture(pieceTexture);
  SDL_DestroyTexture(checkTexture);
  SDL_DestroyTexture(checkMateTexture);
  SDL_DestroyTexture(wonTexture);
  for (int i = 0; i < 8; i++) {
    SDL_DestroyTexture(verticalNotation[i]);
    SDL_DestroyTexture(horizontalNotation[i]);
  }
  SDL_DestroyTexture(matchDrawTexture);
  for (int i = 0; i < 10; i++) {
    SDL_DestroyTexture(numberTextures[i]);
  }
  SDL_DestroyTexture(colonTexture);
  SDL_DestroyTexture(outOfTimeTexture);
  SDL_DestroyTexture(resetButtonTexture);

  SoundManager::clean();
}

void Gameboard::init() {
  setBoard();

  // Some stuff here
  boardStartPos.j = WINDOW_WIDTH / 2 - 4 * BLOCK_WIDTH;
  boardStartPos.i = WINDOW_HEIGHT / 2 - 4 * BLOCK_WIDTH;

  resetButtonTexture = TextureManager::loadSentence("Reset");
  SDL_QueryTexture(resetButtonTexture, NULL, NULL, &resetButtonRect.w,
                   &resetButtonRect.h);

  resetButtonRect.x = boardStartPos.j - resetButtonRect.w - BLOCK_WIDTH / 2;
  resetButtonRect.y = WINDOW_HEIGHT / 2 - resetButtonRect.h / 2;

  // Creating Player Name textures
  for (int i = 0; i < 2; i++) {
    playerNamesTexture[i] =
        TextureManager::loadSentence(state.players[i]->Name.c_str());
  }

  // Load piece Textures
  pieceTexture = TextureManager::loadTexture("assets/pieces.png");

  /*
   *  Load some word textures
   */
  checkTexture = TextureManager::loadSentence("Check");
  checkMateTexture = TextureManager::loadSentence("Checkmate");
  wonTexture = TextureManager::loadSentence("WON");
  matchDrawTexture = TextureManager::loadSentence("Draw");
  for (int i = 0; i < 10; i++) {
    char num[2];
    num[0] = i + ASCII_OFFSET;
    num[1] = '\0';
    numberTextures[i] = TextureManager::loadSentence(num);
  }
  colonTexture = TextureManager::loadSentence(":");
  outOfTimeTexture = TextureManager::loadSentence("Out of time.");

  // load vertical notation textures
  for (int l = 1; l < 9; l++) {
    char no[2];
    no[0] = l + ASCII_OFFSET;
    no[1] = '\0';
    verticalNotation[l - 1] = TextureManager::loadSentence(no, 30);
  }

  // load horizontal notation textures
  for (int l = 0; l < 8; l++) {
    char no[2];
    no[0] = char(l + 97);
    no[1] = '\0';
    horizontalNotation[l] = TextureManager::loadSentence(no, 30);
  }

  SoundManager::init();
}

void Gameboard::setBoard() {
  state = BoardState();
  promotionInfo.promotion = false;
  state.players[0] = new Player(PlayerNames[0], true);
  state.players[1] = new Player(PlayerNames[1], false);

  // Handle FEN string
  Engine::handleFENString(STARTING_FEN, state);
  lastMoveState = lastMoveInfo::None;
  allMoves.clear();
  lastMove.made = false;
  Engine::generateAllMoves(state, allMoves);

  int timeInMinutes = 10;
  // Creating Players
  playerTime[0] = playerTime[1] = timeInMinutes * 60 * FPS;
}

void Gameboard::resetBoard() {
  delete state.players[0];
  delete state.players[1];
  setBoard();
}

void Gameboard::handleMouseDown(SDL_Event &event) {
  // Extract Location
  int x = event.button.x - boardStartPos.j;
  int y = event.button.y - boardStartPos.i;
  Coordinate location = {y / BLOCK_WIDTH, x / BLOCK_WIDTH};

  if (lastMoveState != lastMoveInfo::None &&
      lastMoveState != lastMoveInfo::Check) {
    return;
  }

  if (event.button.button == SDL_BUTTON_LEFT) {
    if (location.isValidBoardIndex()) {
      if (promotionInfo.promotion) {
        if (location.j != promotionInfo.location.j) {
          return;
        }

        int direction = state.isWhiteTurn ? 1 : -1;

        for (int i = 0; i < 4; i++) {
          if (location.i == promotionInfo.location.i + direction * i) {
            moves.push_back({state.dragPieceLocation, promotionInfo.location,
                             Promotion::promotionMap[i + 1]});
            makeMove(promotionInfo.location, state, allMoves, moves,
                     lastMoveState, lastMove, promotionInfo, i + 1);

            promotionInfo.promotion = false;
          }
        }
        return;
      }

      // location.display();
      Piece *piece = state.getPiece(location);
      if (piece) {

        // If it is not the player's turn, do nothing
        if (piece->isWhite() != state.isWhiteTurn) {
          return;
        }

        state.dragPieceId = piece->getID();
        state.dragPieceLocation = location;
        // piece->generateLegalMoves(state, moves);
        Engine::getMovelist(location, allMoves, moves);
      }
    }

  } else if (event.button.button == SDL_BUTTON_RIGHT) {
  }
}

void Gameboard::handleMouseUp(SDL_Event &event) {
  // Extract loaction
  int x = event.button.x - boardStartPos.j;
  int y = event.button.y - boardStartPos.i;
  Coordinate location = {y / BLOCK_WIDTH, x / BLOCK_WIDTH};

  if (event.button.button == SDL_BUTTON_LEFT) {
    if (lastMoveState != lastMoveInfo::None &&
        lastMoveState != lastMoveInfo::Check) {
      if (event.button.x > resetButtonRect.x &&
          event.button.y > resetButtonRect.y &&
          event.button.x < (resetButtonRect.x + resetButtonRect.w) &&
          event.button.y < (resetButtonRect.y + resetButtonRect.h)) {
        resetBoard();
      }
      return;
    }
    makeMove(location, state, allMoves, moves, lastMoveState, lastMove,
             promotionInfo);
    moves.clear();
  }
  state.dragPieceId = 0;
}

void makeMove(Coordinate location, BoardState &state,
              std::vector<std::vector<Move>> &allMoves,
              std::vector<Move> &moves, lastMoveInfo::State &lastMoveState,
              LastMove &lastMove, Promotion::uiInfo &promotionInfo,
              int promotionID) {

  Promotion::promotion promotion = Promotion::None;
  // handlePromotionindex
  if (promotionID >= 0 && promotionID <= 4) {
    promotion = Promotion::promotionMap[promotionID];
  }
  Coordinate temp = state.dragPieceLocation;
  lastMoveInfo info = Engine::handlePiecePlacement(location, state, moves,
                                                   promotionInfo, promotion);

  moves.clear();
  if (info.success) {
    SoundManager::playSound(!state.isWhiteTurn ? SoundManager::WhiteMove
                                               : SoundManager::BlackMove);
    lastMoveState = info.state;
    lastMove.made = true;
    lastMove.startPos = temp;
    lastMove.endPos = location;
    int count = Engine::generateAllMoves(state, allMoves);
    if (count == 0) {
      if (info.state == lastMoveInfo::Check) {
        lastMoveState = lastMoveInfo::CheckMate;
        std::cout << "Checkmate!!!" << std::endl;
      } else {
        lastMoveState = lastMoveInfo::Draw;
      }
    }
  }
}

void Gameboard::update() {
  if ((lastMoveState == lastMoveInfo::None) ||
      (lastMoveState == lastMoveInfo::Check)) {
    playerTime[!state.isWhiteTurn]--;
    if (playerTime[!state.isWhiteTurn] < 0) {
      lastMoveState = lastMoveInfo::OutofTime;
      playerTime[!state.isWhiteTurn] = 0;
    }
  }
}

void Gameboard::render() {
  SDL_Rect notationRect; // vertical board notation
  SDL_Rect destRect;     // where to render
  SDL_Rect srcRect;      // from where we render
  int rightSideRenderingInitialPosition =
      (float)WINDOW_WIDTH / 2 + 4.5 * BLOCK_WIDTH;

  int renderAlpha = promotionInfo.promotion ? 150 : 255;
  if (promotionInfo.promotion) {
    SDL_SetTextureAlphaMod(pieceTexture, renderAlpha);
    SDL_SetTextureAlphaMod(playerNamesTexture[0], renderAlpha);
    SDL_SetTextureAlphaMod(playerNamesTexture[1], renderAlpha);
  }

  // Render board
  destRect.h = destRect.w = BLOCK_WIDTH;
  for (int i = 0; i < 8; i++) {   // row
    for (int j = 0; j < 8; j++) { // column
      if ((i + j) % 2 == 0) {
        SDL_SetRenderDrawColor(Game::renderer, 238, 238, 210,
                               renderAlpha); // cream color
      } else {
        SDL_SetRenderDrawColor(Game::renderer, 118, 150, 86,
                               renderAlpha); // green color
      }
      destRect.x = boardStartPos.j + j * BLOCK_WIDTH;
      destRect.y = boardStartPos.i + i * BLOCK_WIDTH;
      SDL_RenderFillRect(Game::renderer, &destRect);

      // vertical notation
      if (j == 0) {
        SDL_QueryTexture(verticalNotation[8 - i - 1], NULL, NULL,
                         &notationRect.w, &notationRect.h);
        notationRect.x = destRect.x - notationRect.w - 10;
        notationRect.y = destRect.y;
        SDL_RenderCopy(Game::renderer, verticalNotation[8 - i - 1], NULL,
                       &notationRect);
      }

      // horizontal notation
      if (i == 7) {
        SDL_QueryTexture(horizontalNotation[j], NULL, NULL, &notationRect.w,
                         &notationRect.h);
        notationRect.x = destRect.x - (notationRect.w - 55);
        notationRect.y = destRect.y + notationRect.h + 40;
        SDL_RenderCopy(Game::renderer, horizontalNotation[j], NULL,
                       &notationRect);
      }
      if (lastMove.made) {
        if (lastMove.endPos == Coordinate({i, j}) ||
            lastMove.startPos == Coordinate({i, j})) {
          SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 200);
          SDL_RenderFillRect(Game::renderer, &destRect);
        }
      }
    }
  }

  for (Move move : moves) {
    destRect.x = boardStartPos.j + move.endPos.j * BLOCK_WIDTH;
    destRect.y = boardStartPos.i + move.endPos.i * BLOCK_WIDTH;

    if (state.isEmpty(move.endPos) &&
        !(state.enPassantAvailable && state.enPassant == move.endPos)) {
      SDL_SetRenderDrawColor(Game::renderer, 100, 255, 0, renderAlpha);

      // For rendering circles

      // Find center
      destRect.x += BLOCK_WIDTH / 2;
      destRect.y += BLOCK_WIDTH / 2;

      TextureManager::DrawFillCircle(Game::renderer, destRect.x, destRect.y,
                                     BLOCK_WIDTH / 8);
    } else {
      SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 180);
      SDL_RenderFillRect(Game::renderer, &destRect);
    }
  }

  // Render all pieces
  /*
   * TODO: BUG
   * If you try to move the white's queen on top of black's bishop
   * the queen is rendered below the bishop
   * TODO: Make it so that piece being dragged is rendered after all pieces
   * are rendered
   */

  srcRect.h = srcRect.w = 80;
  int capturedPieceOffset[2] = {0, 0};
  int capturedPieceOffsetHeight[2] = {0, 0};
  for (int i = 0; i < 2; i++) {
    for (Piece *piece : state.players[i]->pieces) {
      // If white lower row, if black upper row
      srcRect.y = (piece->isWhite()) ? 0 : srcRect.h;
      srcRect.x = piece->getTextureColumn() * srcRect.h;

      // If the piece is captured, we render it on the side
      if (piece->isCaptured()) {
        int index = piece->isWhite();
        int offsetIncrease = BLOCK_WIDTH / 2;

        destRect.x =
            rightSideRenderingInitialPosition + capturedPieceOffset[index];
        destRect.y =
            (i == 0) ? 0.2 * WINDOW_HEIGHT + capturedPieceOffsetHeight[index]
                     : 0.7 * WINDOW_HEIGHT - capturedPieceOffsetHeight[index];

        // We just chaned destRect's w, h
        destRect.h = destRect.w = offsetIncrease;

        SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);

        destRect.h = destRect.w = BLOCK_WIDTH; // We restored it

        if (destRect.x + capturedPieceOffset[index] > WINDOW_WIDTH) {
          capturedPieceOffset[index] = 0;
          capturedPieceOffsetHeight[index] += offsetIncrease;
        } else {
          capturedPieceOffset[index] += offsetIncrease;
        }

        continue;
      }

      if (piece->getID() == state.dragPieceId) {
        SDL_GetMouseState(&destRect.x, &destRect.y);
        destRect.x -= destRect.w / 2;
        destRect.y -= destRect.h / 2;

        SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);

      } else {
        Coordinate tempCoordinate = piece->getCoordinate();
        destRect.x = boardStartPos.j + tempCoordinate.j * BLOCK_WIDTH;
        destRect.y = boardStartPos.i + tempCoordinate.i * BLOCK_WIDTH;

        SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);
      }
    }
  }

  /*
   * We change the size of destRect from here, be careful
   */

  // Render Player Names
  for (int i = 0; i < 2; i++) {
    destRect.x = rightSideRenderingInitialPosition;
    destRect.y = (i == 0) ? 0.85 * WINDOW_HEIGHT : 0.05 * WINDOW_HEIGHT;
    float timeDataDirection = i == 1 ? 1.1 : -1.1;

    // Gets the size of font width and height
    SDL_QueryTexture(playerNamesTexture[i], NULL, NULL, &destRect.w,
                     &destRect.h);

    SDL_RenderCopy(Game::renderer, playerNamesTexture[i], NULL, &destRect);

    if ((lastMoveState == lastMoveInfo::CheckMate ||
         lastMoveState == lastMoveInfo::OutofTime) &&
        state.isWhiteTurn == i) {
      destRect.x += destRect.w + BLOCK_WIDTH / 4;
      SDL_QueryTexture(wonTexture, NULL, NULL, &destRect.w, &destRect.h);
      SDL_RenderCopy(Game::renderer, wonTexture, NULL, &destRect);
    }

    // Display Time
    destRect.x = rightSideRenderingInitialPosition;
    destRect.y += destRect.h * timeDataDirection;
    int numsToDisplay[4] = {0, 0, 0, 0};
    clockTickToTime(playerTime[i] / FPS, numsToDisplay);

    for (int k = 0; k < 4; k++) {
      SDL_QueryTexture(numberTextures[numsToDisplay[k]], NULL, NULL,
                       &destRect.w, &destRect.h);
      SDL_RenderCopy(Game::renderer, numberTextures[numsToDisplay[k]], NULL,
                     &destRect);
      destRect.x += destRect.w;
      if (k == 1) {
        SDL_QueryTexture(colonTexture, NULL, NULL, &destRect.w, &destRect.h);
        SDL_RenderCopy(Game::renderer, colonTexture, NULL, &destRect);
        destRect.x += destRect.w;
      }
    }
  }

  destRect.x = rightSideRenderingInitialPosition;
  destRect.y = 0.45 * WINDOW_HEIGHT;
  switch (lastMoveState) {
  case lastMoveInfo::OutofTime: {
    SDL_QueryTexture(outOfTimeTexture, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderCopy(Game::renderer, outOfTimeTexture, NULL, &destRect);
    break;
  }
  case lastMoveInfo::CheckMate: {
    SDL_QueryTexture(checkMateTexture, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderCopy(Game::renderer, checkMateTexture, NULL, &destRect);
    break;
  }
  case lastMoveInfo::Check: {
    SDL_QueryTexture(checkTexture, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderCopy(Game::renderer, checkTexture, NULL, &destRect);
    break;
  }
  case lastMoveInfo::Draw: {
    SDL_QueryTexture(matchDrawTexture, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderCopy(Game::renderer, matchDrawTexture, NULL, &destRect);
  }

  default:
    break;
    ;
  }
  if (lastMoveState != lastMoveInfo::None &&
      lastMoveState != lastMoveInfo::Check) {
    SDL_RenderCopy(Game::renderer, resetButtonTexture, NULL, &resetButtonRect);
  }

  // Render Promotion Info
  if (promotionInfo.promotion) {
    destRect.y = boardStartPos.i + promotionInfo.location.i * BLOCK_WIDTH;
    destRect.x = boardStartPos.j + promotionInfo.location.j * BLOCK_WIDTH;
    destRect.w = destRect.h = BLOCK_WIDTH;

    srcRect.h = srcRect.w = 80;
    srcRect.y = (state.isWhiteTurn) ? 0 : srcRect.h;

    SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255);
    SDL_SetTextureAlphaMod(pieceTexture, 255);

    for (int i = 1; i < 5; i++) {
      if (!(i == 1)) {
        destRect.y += state.isWhiteTurn ? BLOCK_WIDTH : -BLOCK_WIDTH;
      }
      SDL_RenderFillRect(Game::renderer, &destRect);
      srcRect.x = i * srcRect.h;
      SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);
    }
  }
}

// TODO
void Gameboard::handleInput(SDL_Event &event) {
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    handleMouseDown(event);
    break;
  case SDL_MOUSEBUTTONUP:
    handleMouseUp(event);
    break;
  }
}
void Gameboard::pause() {}
void Gameboard::resume() {}

void clockTickToTime(int clock, int timeInfo[4]) {
  int min, secs;
  min = clock / 60;
  secs = clock % 60;

  timeInfo[0] = min / 10;
  timeInfo[1] = min % 10;
  timeInfo[2] = secs / 10;
  timeInfo[3] = secs % 10;
}
