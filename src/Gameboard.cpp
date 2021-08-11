#include "../headers/Gameboard.h"
#include "../headers/Engine.h"
#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace Promotion {
std::map<int, promotion> promotionMap{
    {0, None}, {1, Queen}, {2, Bishop}, {3, Knight}, {4, Rook}};
}

void makeMove(Coordinate location, BoardState &state,
              std::vector<std::vector<Move>> &allMoves,
              std::vector<Move> &moves, Promotion::uiInfo &promotionInfo,
              int promotionID = 0);

Gameboard::Gameboard() {}

Gameboard::~Gameboard() {
  SDL_DestroyTexture(playerNamesTexture[0]);
  SDL_DestroyTexture(playerNamesTexture[1]);
  SDL_DestroyTexture(pieceTexture);
}

void Gameboard::init() {
  state = BoardState();
  promotionInfo.promotion = false;

  // Some stuff here
  boardStartPos.j = WINDOW_WIDTH / 2 - 4 * BLOCK_WIDTH;
  boardStartPos.i = WINDOW_HEIGHT / 2 - 4 * BLOCK_WIDTH;

  // Creating Players
  state.players[0] = new Player("Suban", true);
  state.players[1] = new Player("Prabin", false);

  // Creating Player Name textures
  for (int i = 0; i < 2; i++) {
    playerNamesTexture[i] =
        TextureManager::loadSentence(state.players[i]->Name.c_str());
  }

  // Handle FEN string
  Engine::handleFENString(STARTING_FEN, state);

  // Load piece Textures
  pieceTexture = TextureManager::loadTexture("assets/pieces.png");

  allMoves.clear();
  Engine::generateAllMoves(state, allMoves);
}

void Gameboard::handleMouseDown(SDL_Event &event) {
  // Extract Location
  int x = event.button.x - boardStartPos.j;
  int y = event.button.y - boardStartPos.i;
  Coordinate location = {y / BLOCK_WIDTH, x / BLOCK_WIDTH};

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
                     promotionInfo, i + 1);
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
    makeMove(location, state, allMoves, moves, promotionInfo);
    moves.clear();
  }
  state.dragPieceId = 0;
}

void makeMove(Coordinate location, BoardState &state,
              std::vector<std::vector<Move>> &allMoves,
              std::vector<Move> &moves, Promotion::uiInfo &promotionInfo,
              int promotionID) {

  Promotion::promotion promotion = Promotion::None;
  // handlePromotionindex
  if (promotionID >= 0 && promotionID <= 4) {
    promotion = Promotion::promotionMap[promotionID];
  }

  bool success = Engine::handlePiecePlacement(location, state, moves,
                                              promotionInfo, promotion);

  if (success) {
    SoundManager::playSound(!state.isWhiteTurn ? SoundManager::WhiteMove
                                               : SoundManager::BlackMove);
  }
  moves.clear();
  if (success) {
    int count = Engine::generateAllMoves(state, allMoves);
    if (count == 0) {
      std::cout << "Checkmate!!!" << std::endl;
    }
  }
}

void Gameboard::update() {}

void Gameboard::render() {

  SDL_Rect destRect; // where to render
  SDL_Rect srcRect;  // from where we render

  u_int8_t renderAlpha = promotionInfo.promotion ? 150 : 255;
  if (promotionInfo.promotion) {
    SDL_SetTextureAlphaMod(pieceTexture, renderAlpha);
    SDL_SetTextureAlphaMod(playerNamesTexture[0], renderAlpha);
    SDL_SetTextureAlphaMod(playerNamesTexture[1], renderAlpha);
  }

  // Render board
  destRect.h = destRect.w = BLOCK_WIDTH;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((i + j) % 2 == 0) {
        SDL_SetRenderDrawColor(Game::renderer, 238, 238, 210, renderAlpha);
      } else {
        SDL_SetRenderDrawColor(Game::renderer, 118, 150, 86, renderAlpha);
      }
      destRect.x = boardStartPos.j + j * BLOCK_WIDTH;
      destRect.y = boardStartPos.i + i * BLOCK_WIDTH;
      SDL_RenderFillRect(Game::renderer, &destRect);

      if (lastMove.made) {
        if (lastMove.endPos == Coordinate({j, i}) || lastMove.startPos == Coordinate({j,i})) {
          SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 200);
          SDL_RenderFillRect(Game::renderer, &destRect);
        }
      }

    }
  }

  // Highlight all the possible moes
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

  srcRect.h = srcRect.w = 200;
  int capturedPieceOffset[2] = {0, 0};
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
            WINDOW_WIDTH / 2 + 5 * BLOCK_WIDTH + capturedPieceOffset[index];
        destRect.y = (i == 0) ? 0.2 * WINDOW_HEIGHT : 0.7 * WINDOW_HEIGHT;

        // We just chaned destRect's w, h
        destRect.h = destRect.w = offsetIncrease;

        SDL_RenderCopy(Game::renderer, pieceTexture, &srcRect, &destRect);

        destRect.h = destRect.w = BLOCK_WIDTH; // We restored it

        capturedPieceOffset[index] += offsetIncrease;
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
  destRect.x = WINDOW_WIDTH / 2 + 5 * BLOCK_WIDTH;
  for (int i = 0; i < 2; i++) {
    destRect.y = (i == 0) ? 0.1 * WINDOW_HEIGHT : 0.8 * WINDOW_HEIGHT;

    // Gets the size of font width and height
    SDL_QueryTexture(playerNamesTexture[i], NULL, NULL, &destRect.w,
                     &destRect.h);

    SDL_RenderCopy(Game::renderer, playerNamesTexture[i], NULL, &destRect);
  }

  // Render Promotion Info
  if (promotionInfo.promotion) {
    destRect.y = boardStartPos.i + promotionInfo.location.i * BLOCK_WIDTH;
    destRect.x = boardStartPos.j + promotionInfo.location.j * BLOCK_WIDTH;
    destRect.w = destRect.h = BLOCK_WIDTH;

    srcRect.h = srcRect.w = 200;
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
