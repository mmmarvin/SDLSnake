#include <SDL2/SDL.h>
#include "clock.h"

void startClock(struct Clock* clock)
{
  clock->start = SDL_GetTicks();
}

int elapsedClock(struct Clock* clock)
{
  return SDL_GetTicks() - clock->start;
}

void resetClock(struct Clock* clock)
{
  startClock(clock);
}
