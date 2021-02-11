#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>

extern SDL_Window*    gameRenderWindow;
extern SDL_Renderer*  gameRenderer;

void closeRenderWindow();
int initRenderWindow();

#endif // RENDERER_H
