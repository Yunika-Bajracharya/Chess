#include "../headers/Gameboard.h"
#include "../headers/Engine.h"
#include <SDL2/SDL_render.h>
#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace Promotion {
std::map<int, promotion> promotionMap{
    {0, None}, {1, Queen}, {2, Bishop}, {3, Knight}, {4, Rook}};
}

void clockTickToTime(int clock, int timeInfo[4]);
bool buttonPress(int x, int y, const SDL_Rect &rect);
std::string scoreToString(float score);

Gameboard::Gameboard(Game *_gameRef, std::string name1, std::string name2,
                     int _startTimeInMinutes, bool _useEngine)
    : gameRef(_gameRef), startTimeInMinutes(_startTimeInMinutes) {
  PlayerNames[0] = name1;
  PlayerNames[1] = name2;
  score[0] = score[1] = 0;
  useEngine = _useEngine;
  enginePlaysWhite = false;
}

Gameboard::~Gameboard() { SoundManager::clean(); }

void Gameboard::init() {

  setBoard();
  Gameboard::loadImg();
  // reset, exit and resign button
  boardStartPos.j = WINDOW_WIDTH / 2 - 4 * BLOCK_WIDTH;
  boardStartPos.i = WINDOW_HEIGHT / 2 - 4 * BLOCK_WIDTH;

  resetButtonRect.w = resetButtonTexture.getWidth();
  resetButtonRect.h = resetButtonTexture.getHeight();
  resetButtonRect.x = WINDOW_WIDTH * 0.08;
  resetButtonRect.y = WINDOW_HEIGHT * 0.65;

  exitButtionTexture.queryTexture(exitButtionRect.w, exitButtionRect.h);
  exitButtionRect.x = WINDOW_WIDTH * 0.08;
  exitButtionRect.y = WINDOW_HEIGHT * 0.85;

  resignButtonTexture.queryTexture(resignButtonRect.w, resignButtonRect.h);
  resignButtonRect.x = WINDOW_WIDTH * 0.08;
  resignButtonRect.y = WINDOW_HEIGHT * 0.75;

  // Creating Player Name textures
  for (int i = 0; i < 2; i++) {
    playerNamesTexture[i].loadSentence(state.players[i]->Name.c_str(), 21);
  }

  // Load piece Textures
  pieceTexture.loadFromFile("assets/pieces.png");

  for (int i = 0; i < 10; i++) {
    char num[2];
    num[0] = i + ASCII_OFFSET;
    num[1] = '\0';
    numberTextures[i].loadSentence(num, 28);
  }
  colonTexture.loadSentence(":", 28);

  // load vertical notation textures
  for (int l = 1; l < 9; l++) {
    char no[2];
    no[0] = l + ASCII_OFFSET;
    no[1] = '\0';
    verticalNotation[l - 1].loadSentence(no, 25);
  }

  // load horizontal notation textures
  for (int l = 0; l < 8; l++) {
    char no[2];
    no[0] = char(l + 97);
    no[1] = '\0';
    horizontalNotation[l].loadSentence(no, 25);
  }

  SoundManager::init();

  // Engine::setEngineDifficulty(Engine::Random);
}

// loading images
void Gameboard::loadImg() {
  wonTexture.loadFromFile("./assets/crown.png");
  resetButtonTexture.loadFromFile("./assets/reset.png");
  checkTexture.loadFromFile("./assets/check.png");
  checkMateTexture.loadFromFile("./assets/checkmate.png");
  matchDrawTexture.loadFromFile("./assets/draw.png");
  outOfTimeTexture.loadFromFile("./assets/timeup.png");
  resignButtonTexture.loadFromFile("./assets/resign.png");
  blackResignTexture.loadFromFile("./assets/blackResign.png");
  whiteResignTexture.loadFromFile("./assets/whiteResign.png");
  exitButtionTexture.loadFromFile("./assets/exitGameBoard.png");
  scoreboardTexture.loadFromFile("./assets/scoreboard.png");
}

void Gameboard::setBoard() {
  state = BoardState();
  promotionInfo.promotion = false;
  state.players[0] = new Player(PlayerNames[0], true);
  state.players[1] = new Player(PlayerNames[1], false);
  hasPlayedMove[0] = false;
  hasPlayedMove[1] = false;
  // load score texture
  scoreTexture[0].loadSentence(scoreToString(score[0]), 30,
                               TextureManager::darkGreen);
  scoreTexture[1].loadSentence(scoreToString(score[1]), 30,
                               TextureManager::darkGreen);

  // Handle FEN string
  Engine::handleFENString(STARTING_FEN, state);
  lastMoveState = lastMoveInfo::None;
  allMoves.clear();
  lastMove.made = false;
  Engine::generateAllMoves(state, allMoves);

  // Creating Players
  playerTime[0] = playerTime[1] = startTimeInMinutes * 60 * FPS;

  if (enginePlaysWhite && useEngine) {
    Gameboard::engineMove();
  }
}

void Gameboard::resetBoard() {
  // We swap the names, namesTexture and score texture after game is reset
  std::string tempStr;
  float tempInt;
  tempInt = score[1];
  score[1] = score[0];
  score[0] = tempInt;
  tempStr = PlayerNames[1];
  PlayerNames[1] = PlayerNames[0];
  PlayerNames[0] = tempStr;
  resetScoreTexture();
  enginePlaysWhite = !enginePlaysWhite;

  delete state.players[0];
  delete state.players[1];
  setBoard();
}
void Gameboard::resetScoreTexture() {
  playerNamesTexture[0].loadSentence(PlayerNames[0], 21);
  playerNamesTexture[1].loadSentence(PlayerNames[1], 21);
}

// when mouse is clicked
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
            makeMove(promotionInfo.location, i + 1);

            promotionInfo.promotion = false;

            if (useEngine && enginePlaysWhite == state.isWhiteTurn) {
              Gameboard::engineMove();
            }
            break;
          }
        }
        return;
      }

      Piece *piece = state.getPiece(location);
      if (piece) {

        // If it is not the player's turn, do nothing
        if (piece->isWhite() != state.isWhiteTurn) {
          return;
        }

        state.dragPieceId = piece->getID();
        state.dragPieceLocation = location;
        Engine::getMovelist(location, allMoves, moves);
      }
    }

  } else if (event.button.button == SDL_BUTTON_RIGHT) {
  }
}

// when unclicking the mouse
void Gameboard::handleMouseUp(SDL_Event &event) {
  // Extract loaction
  int x = event.button.x - boardStartPos.j;
  int y = event.button.y - boardStartPos.i;
  Coordinate location = {y / BLOCK_WIDTH, x / BLOCK_WIDTH};

  if (event.button.button == SDL_BUTTON_LEFT) {
    if (buttonPress(event.button.x, event.button.y, exitButtionRect)) {
      Gameboard::goToMainMenu();
      return;
    }
    if (buttonPress(event.button.x, event.button.y, resignButtonRect)) {
      if (!(lastMoveState == lastMoveInfo::CheckMate ||
            lastMoveState == lastMoveInfo::Draw))
        Gameboard::resign();
    }
    if (lastMoveState != lastMoveInfo::None &&
        lastMoveState != lastMoveInfo::Check) {
      if (buttonPress(event.button.x, event.button.y, resetButtonRect)) {
        resetBoard();
      }
      return;
    }
    bool success = makeMove(location);
    moves.clear();

    if (success) {
      hasPlayedMove[state.isWhiteTurn] = true;
      if (useEngine && enginePlaysWhite == state.isWhiteTurn) {
        Gameboard::engineMove();
      }
    }
  }
  state.dragPieceId = 0;
}

bool Gameboard::makeMove(Coordinate location, int promotionID) {

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
        score[state.isWhiteTurn] += 1;
      } else {
        lastMoveState = lastMoveInfo::Draw;
        score[0] += 0.5;
        score[1] += 0.5;
      }
      Gameboard::resetScoreTexture();
    }
  }
  return info.success;
}

void Gameboard::update() {
  if ((lastMoveState == lastMoveInfo::None) ||
      (lastMoveState == lastMoveInfo::Check)) {
    if (!hasPlayedMove[!state.isWhiteTurn])
      return;

    playerTime[!state.isWhiteTurn]--;
    if (playerTime[!state.isWhiteTurn] < 0) {
      lastMoveState = lastMoveInfo::OutofTime;
      playerTime[!state.isWhiteTurn] = 0;
      score[0] += 1;
    }
  }
}

void Gameboard::render() {
  SDL_Rect notationRect; // vertical board notation
  SDL_Rect destRect;     // where to render
  SDL_Rect srcRect;      // from where we render
  int rightSideRenderingInitialPosition =
      (float)WINDOW_WIDTH / 2 + 4.5 * BLOCK_WIDTH;
  int posX, posY;

  int renderAlpha = promotionInfo.promotion ? 150 : 255;
  if (promotionInfo.promotion) {
    pieceTexture.setAlpha(renderAlpha);
    playerNamesTexture[0].setAlpha(renderAlpha);
    playerNamesTexture[1].setAlpha(renderAlpha);
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
        verticalNotation[8 - i - 1].queryTexture(notationRect.w,
                                                 notationRect.h);
        notationRect.x = destRect.x - notationRect.w - 10;
        notationRect.y = destRect.y;
        verticalNotation[8 - i - 1].render(&notationRect);
      }

      // horizontal notation
      if (i == 7) {
        horizontalNotation[j].queryTexture(notationRect.w, notationRect.h);
        notationRect.x = destRect.x - (notationRect.w - 55);
        notationRect.y = destRect.y + notationRect.h + 50;
        horizontalNotation[j].render(&notationRect);
      }
      if (lastMove.made) {
        if (lastMove.endPos == Coordinate({i, j}) ||
            lastMove.startPos == Coordinate({i, j})) {
          SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 150);
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

        pieceTexture.render(&destRect, &srcRect);

        destRect.h = destRect.w = BLOCK_WIDTH; // We restored it

        if (destRect.x + capturedPieceOffset[index] > WINDOW_WIDTH) {
          capturedPieceOffset[index] = 0;
          capturedPieceOffsetHeight[index] += offsetIncrease;
        } else {
          capturedPieceOffset[index] += offsetIncrease;
        }

        continue;
      }

      if (!(piece->getID() == state.dragPieceId)) {
        Coordinate tempCoordinate = piece->getCoordinate();
        destRect.x = boardStartPos.j + tempCoordinate.j * BLOCK_WIDTH;
        destRect.y = boardStartPos.i + tempCoordinate.i * BLOCK_WIDTH;
        pieceTexture.render(&destRect, &srcRect);
      }
    }
  }
  // Render Dragged piece
  Piece *draggedPiece = state.getPiece(state.dragPieceId);
  if (draggedPiece) {
    SDL_GetMouseState(&destRect.x, &destRect.y);
    destRect.x -= destRect.w / 2;
    destRect.y -= destRect.h / 2;
    srcRect.y = (draggedPiece->isWhite()) ? 0 : srcRect.h;
    srcRect.x = draggedPiece->getTextureColumn() * srcRect.h;
    pieceTexture.render(&destRect, &srcRect);
  }

  /*
   * We change the size of destRect from here, be careful
   */

  // Render Player Names
  for (int i = 0; i < 2; i++) {
    posX = rightSideRenderingInitialPosition;
    // if yes for white, if not for black
    posY = (i == 0) ? 0.91 * WINDOW_HEIGHT : 0.05 * WINDOW_HEIGHT;
    float timeDataDirection = i == 1 ? 1.1 : -1.1;
    playerNamesTexture[i].render(posX, posY);

    if ((lastMoveState == lastMoveInfo::CheckMate ||
         lastMoveState == lastMoveInfo::OutofTime ||
         lastMoveState == lastMoveInfo::Resign) &&
        state.isWhiteTurn == i) {
      posX += playerNamesTexture[i].getWidth();
      posY = (i == 0) ? 0.9 * WINDOW_HEIGHT : 0.05 * WINDOW_HEIGHT;
      wonTexture.render(posX, posY);
    }

    // Display Time
    posX = rightSideRenderingInitialPosition;
    posY += playerNamesTexture[i].getHeight() * timeDataDirection;
    int numsToDisplay[4] = {0, 0, 0, 0};
    clockTickToTime(playerTime[i] / FPS, numsToDisplay);

    for (int k = 0; k < 4; k++) {
      numberTextures[numsToDisplay[k]].render(posX, posY);
      posX += numberTextures[numsToDisplay[k]].getWidth();
      if (k == 1) {
        colonTexture.render(posX, posY);
        posX += colonTexture.getWidth();
      }
    }

    // display score board
    posX = WINDOW_WIDTH * 0.06;
    posY = WINDOW_HEIGHT * 0.05;
    scoreboardTexture.render(posX, posY);

    for (int i = 0; i < 2; i++) {

      // display score texture
      posX = (i == 0) ? 0.082 * WINDOW_WIDTH : 0.16 * WINDOW_WIDTH;
      posY = WINDOW_HEIGHT * 0.18;
      scoreTexture[i].render(posX, posY);

      // display player names in score board
      posX = (i == 0) ? 0.072 * WINDOW_WIDTH : 0.148 * WINDOW_WIDTH;
      posY = WINDOW_HEIGHT * 0.13;
      playerNamesTexture[i].render(posX, posY);
    }
  }

  posX = rightSideRenderingInitialPosition;
  posY = 0.45 * WINDOW_HEIGHT;
  switch (lastMoveState) {
  case lastMoveInfo::OutofTime: {
    outOfTimeTexture.render(posX, posY);
    break;
  }
  case lastMoveInfo::CheckMate: {
    checkMateTexture.render(posX, posY);
    break;
  }
  case lastMoveInfo::Check: {
    checkTexture.render(posX, posY);
    break;
  }
  case lastMoveInfo::Draw: {
    matchDrawTexture.render(posX, posY);
    break;
  }
  case lastMoveInfo::Resign: {
    if (state.isWhiteTurn) {
      whiteResignTexture.render(posX, posY);
    } else {
      blackResignTexture.render(posX, posY);
    }
    break;
  }

  default:
    break;
  }
  if (lastMoveState != lastMoveInfo::None &&
      lastMoveState != lastMoveInfo::Check) {
    resetButtonTexture.render(&resetButtonRect);
  }

  // Render Promotion Info
  if (promotionInfo.promotion) {
    destRect.y = boardStartPos.i + promotionInfo.location.i * BLOCK_WIDTH;
    destRect.x = boardStartPos.j + promotionInfo.location.j * BLOCK_WIDTH;
    destRect.w = destRect.h = BLOCK_WIDTH;

    srcRect.h = srcRect.w = 80;
    srcRect.y = (state.isWhiteTurn) ? 0 : srcRect.h;

    SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255);
    pieceTexture.setAlpha(255);

    for (int i = 1; i < 5; i++) {
      if (!(i == 1)) {
        destRect.y += state.isWhiteTurn ? BLOCK_WIDTH : -BLOCK_WIDTH;
      }
      SDL_RenderFillRect(Game::renderer, &destRect);
      srcRect.x = i * srcRect.h;
      pieceTexture.render(&destRect, &srcRect);
    }
  }

  exitButtionTexture.render(&exitButtionRect);

  if (!(lastMoveState == lastMoveInfo::CheckMate ||
        lastMoveState == lastMoveInfo::Draw))
    resignButtonTexture.render(&resignButtonRect);
}

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

void Gameboard::goToMainMenu() { gameRef->goBackToGameMenu(); }

void Gameboard::resign() {
  lastMoveState = lastMoveInfo::Resign;
  score[state.isWhiteTurn] += 1;
}

void Gameboard::engineMove() {
  Move *move = Engine::generateAIMove(state, allMoves);
  if (!move) {
    return;
  }
  bool check = Engine::placePiece(*move, state);
  lastMoveState = check ? lastMoveInfo::Check : lastMoveInfo::None;

  int count = Engine::generateAllMoves(state, allMoves);
  if (count == 0) {
    if (check) {
      lastMoveState = lastMoveInfo::CheckMate;
    } else {
      lastMoveState = lastMoveInfo::Draw;
    }
  }
  lastMove.made = true;
  lastMove.startPos = move->startPos;
  lastMove.endPos = move->endPos;

  delete move;
  return;
}

void clockTickToTime(int clock, int timeInfo[4]) {
  int min, secs;
  min = clock / 60;
  secs = clock % 60;

  timeInfo[0] = min / 10;
  timeInfo[1] = min % 10;
  timeInfo[2] = secs / 10;
  timeInfo[3] = secs % 10;
}

std::string scoreToString(float score) {
  std::string str;
  str.clear();
  int beforePoint = score;
  if (beforePoint > 99) {
    beforePoint = 99;
  }
  str = std::to_string(beforePoint);
  str += '.';
  if ((score - beforePoint) * 2 >= 1) {
    str += '5';
  } else {
    str += '0';
  }
  return str;
}
