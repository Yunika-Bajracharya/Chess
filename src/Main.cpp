#include "../headers/Game.h"
#include "../headers/Test.h"
#include <chrono>

Game *game = nullptr;
int main(int argc, char *argv[]) {
  if (argc == 1) {
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
               WINDOW_WIDTH, WINDOW_HEIGHT, false);

    while (game->running()) {

      frameStart = SDL_GetTicks();

      game->handleEvents();
      game->update();
      game->render();

      frameTime = SDL_GetTicks() - frameStart;

      if (frameDelay > frameTime) {
        SDL_Delay(frameDelay - frameTime);
      }
    }
    delete game;
    return 0;
  }

  if (argc == 2) {
    Test t;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    for (int i = 0; i < 5; i++) {
      std::cout << "Depth: " << i + 1 << std::endl;
      std::cout << "Number of Positions: ";

      auto t1 = high_resolution_clock::now();
      t.generateAllMoves(i, false);
      auto t2 = high_resolution_clock::now();

      auto ms_int = duration_cast<milliseconds>(t2 - t1);

      std::cout << "Time taken: " << ms_int.count() << " ms" << std::endl;
    }
  }
  if (argc == 3) {
    Test t;
    t.generateAllMoves(DEPTH, true);
  }
}
