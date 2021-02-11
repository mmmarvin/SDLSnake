#include "game.h"
#include "render.h"
#include "queue.h"

int main()
{
  if(initRenderWindow())
    return 1;
  runGameLoop();

  closeRenderWindow();
  return 0;
}
