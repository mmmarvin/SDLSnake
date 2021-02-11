#include <stdio.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "clock.h"
#include "colors.h"
#include "define.h"
#include "game.h"
#include "queue.h"
#include "render.h"

enum Direction { Left, Right, Up, Down };

int gameIsRunning = 1;
int gameOver = 0;
int** gameData = NULL;

int gameScore = 0;
int gameSpeed = STARTING_SPEED;

struct Clock gameClock;

struct Queue snake;
int snakeDirection = Right;

static void initializeSnake()
{
  initQueue(&snake);
}

static void createGameDataWalls()
{
  for(int i = 0; i < WORLD_WIDTH; ++i)
    gameData[0][i] = 1;
  for(int i = 0; i < WORLD_WIDTH; ++i)
    gameData[WORLD_HEIGHT - 1][i] = 1;

  for(int i = 0; i < WORLD_HEIGHT; ++i)
    gameData[i][0] = 1;
  for(int i = 0; i < WORLD_HEIGHT; ++i)
    gameData[i][WORLD_WIDTH - 1] = 1;
}

static void createRandomFood()
{
  int maxWidth = WORLD_WIDTH - 2;
  int maxHeight = WORLD_HEIGHT - 2;

  int x = 1 + rand() % maxWidth;
  int y = 1 + rand() % maxHeight;

  while(gameData[y][x]) {
    x = 1 + rand() % maxWidth;
    y = 1 + rand() % maxHeight;
  }

  gameData[y][x] = 2;
}

static void resetSnake()
{
  queueDelete(&snake);
  initializeSnake();
}

static void resetGameData()
{
  resetSnake();

  for(int j = 0; j < WORLD_HEIGHT; ++j)
    for(int i = 0; i < WORLD_WIDTH; ++i)
      gameData[j][i] = 0;

  gameData[WORLD_HEIGHT / 2][(WORLD_WIDTH / 2)] = 1;
  gameData[WORLD_HEIGHT / 2][(WORLD_WIDTH / 2) + 1] = 1;
  gameData[WORLD_HEIGHT / 2][(WORLD_WIDTH / 2) + 2] = 1;
  gameData[WORLD_HEIGHT / 2][(WORLD_WIDTH / 2) + 3] = 1;

  struct Vector2 vec = { (WORLD_WIDTH / 2), (WORLD_HEIGHT / 2) };
  queuePush(&snake, vec);

  vec.x = (WORLD_WIDTH / 2) + 1;
  vec.y = (WORLD_HEIGHT / 2);
  queuePush(&snake, vec);

  vec.x = (WORLD_WIDTH / 2) + 2;
  vec.y = (WORLD_HEIGHT / 2);
  queuePush(&snake, vec);

  vec.x = (WORLD_WIDTH / 2) + 3;
  vec.y = (WORLD_HEIGHT / 2);
  queuePush(&snake, vec);

  createRandomFood();

  gameScore = 0;
  gameSpeed = STARTING_SPEED;
  snakeDirection = Right;
}

static void createGameData()
{
  gameData = (int**) malloc(sizeof(int*) * WORLD_HEIGHT);
  for(int i = 0; i < WORLD_HEIGHT; ++i)
    gameData[i] = (int*) malloc(sizeof(int) * WORLD_WIDTH);
}

static void deleteGameData()
{
  for(int i = 0; i < WORLD_HEIGHT; ++i)
    free(gameData[i]);
  free(gameData);
}

static void moveSnake(int x, int y)
{
  if(!gameOver) {
    if(!gameData[y][x]) {
      gameData[y][x] = 1;

      struct Vector2 newHead = { x, y };
      queuePush(&snake, newHead);

      struct Vector2 tail = queuePop(&snake);
      gameData[tail.y][tail.x] = 0;
    } else if(gameData[y][x] == 2) {
      gameData[y][x] = 1;

      struct Vector2 newHead = { x, y };
      queuePush(&snake, newHead);

      gameScore -= SPEED_INCREASER;
      gameSpeed += SPEED_INCREASER;
      gameSpeed = gameSpeed < 1 ? 1 : gameSpeed;
      createRandomFood();
    } else {
      gameOver = 1;
    }
  }
}

static int countDigits(int n)
{
  int count = 0;
  while(n) {
    n /= 10;
    ++count;
  }

  return count;
}

void render();
void update();
void handleInput();

void runGameLoop()
{
  struct Clock timeStepClock;

  srand(time(NULL));

  TTF_Init();
  TTF_Font* scoreFont = TTF_OpenFont("DejaVuSans.ttf", 12);

  initializeSnake();
  createGameData();

  resetGameData();
  createGameDataWalls();

  startClock(&gameClock);
  startClock(&timeStepClock);

  int accumulator = 0;
  while(gameIsRunning) {
    int elapsed = elapsedClock(&timeStepClock);
    resetClock(&timeStepClock);

    accumulator += elapsed;
    while(accumulator >= 17) {
      update();
      handleInput();
      accumulator -= elapsed;
    }

    char scoreChar[255];
    sprintf(&scoreChar[0], "%d", gameScore);

    SDL_Surface* scoreText = TTF_RenderText_Solid(scoreFont, &scoreChar[0], COLOR_BLACK);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(gameRenderer, scoreText);

    SDL_SetRenderDrawColor(gameRenderer, COLOR_WHITE.r, COLOR_WHITE.b, COLOR_WHITE.b, COLOR_WHITE.a);
    SDL_RenderClear(gameRenderer);
    render();

    int digitCount = gameScore == 0 ? 1 : countDigits(gameScore);

    SDL_Rect scoreRect;
    scoreRect.x = ((WORLD_WIDTH / 2) * PIXEL_SIZE) - (digitCount * (WORLD_WIDTH / 4));
    scoreRect.y = 10;
    scoreRect.w = WORLD_WIDTH * digitCount;
    scoreRect.h = WORLD_HEIGHT;

    SDL_RenderCopy(gameRenderer, scoreTexture, NULL, &scoreRect);
    SDL_RenderPresent(gameRenderer);

    SDL_DestroyTexture(scoreTexture);
  }

  TTF_Quit();

  deleteGameData();
  resetSnake();
  closeRenderWindow();
}

void render()
{
  struct SDL_Rect rect;

  for(int j = 0; j < WORLD_HEIGHT; ++j) {
    for(int i = 0; i < WORLD_WIDTH; ++i) {
      rect.x = i * PIXEL_SIZE;
      rect.y = j * PIXEL_SIZE;
      rect.w = PIXEL_SIZE;
      rect.h = PIXEL_SIZE;

      if(gameData[j][i]) {
        SDL_SetRenderDrawColor(gameRenderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
        SDL_RenderFillRect(gameRenderer, &rect);
      }
    }
  }
}

void update()
{
  if(elapsedClock(&gameClock) > gameSpeed) {
    struct Vector2 head = queueBottom(&snake);

    switch(snakeDirection) {
    case Left:
      moveSnake(head.x - 1, head.y);
      break;
    case Right:
      moveSnake(head.x + 1, head.y);
      break;
    case Up:
      moveSnake(head.x, head.y - 1);
      break;
    case Down:
      moveSnake(head.x, head.y + 1);
      break;
    }

    resetClock(&gameClock);
  }
}

void handleInput()
{
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
    case SDL_QUIT:
        gameIsRunning = 0;
      break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
      case SDLK_UP:
        if(snakeDirection != Down)
          snakeDirection = Up;
        break;
      case SDLK_DOWN:
        if(snakeDirection != Up)
          snakeDirection = Down;
        break;
      case SDLK_LEFT:
        if(snakeDirection != Right)
          snakeDirection = Left;
        break;
      case SDLK_RIGHT:
        if(snakeDirection != Left)
          snakeDirection = Right;
        break;
      case SDLK_SPACE:
        if(gameOver) {
          resetGameData();
          createGameDataWalls();
          gameOver = 0;
        }
      }
      break;
    }
  }
}
