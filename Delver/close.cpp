/*
	Author: Nathan Smith
	This file contains the function defniitions for closing the program,
	freeing memory of the renderer and the window.
*/

#include <SDL.h>
#include "prototypes.h"

void close(SDL_Renderer **renderer, SDL_Window **window) {
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	*renderer = NULL;
	*window = NULL;

	SDL_Quit();
}