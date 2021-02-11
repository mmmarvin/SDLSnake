#include "define.h"
#include "render.h"

SDL_Window*   gameRenderWindow = NULL;
SDL_Renderer* gameRenderer = NULL;

void closeRenderWindow()
{
  if(gameRenderWindow)
    SDL_DestroyWindow(gameRenderWindow);
  SDL_Quit();

  gameRenderWindow = NULL;
  gameRenderer = NULL;
}

int initRenderWindow()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Cannot init SDL");
    return 1;
  }

  gameRenderWindow = SDL_CreateWindow("SDL Snake",
                                      100,
                                      100,
                                      WORLD_WIDTH * PIXEL_SIZE,
                                      WORLD_HEIGHT * PIXEL_SIZE,
                                      SDL_WINDOW_SHOWN);
  if(gameRenderWindow == NULL) {
    printf("Cannot create SDL Window");
    closeRenderWindow();
    return 1;
  }

  gameRenderer = SDL_CreateRenderer(gameRenderWindow,
                                    -1,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(gameRenderer == NULL) {
    printf("Cannot create SDL Renderer");
    closeRenderWindow();
    return 1;
  }

  return 0;
}
