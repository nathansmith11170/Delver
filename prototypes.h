/*
	Author: Nathan Smith

	This file contains the function prototypes for the Delver project
*/

#include <SDL.h>
#include <map>
#include "LTexture.h"
#include "MatrixGraph.h"

#ifndef FUNCTIONS_AND_CLASSES
#define FUNCTIONS_AND_CLASSES

/*
	This class encapsulates a texture, which is loaded from a file. It supports
	rendering of a clipped portion of the texture, rather than the entirety.
	It holds its own reference to the renderer.
*/
MatrixGraph mazeGenerate(int NODES);

bool init(int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Renderer **gRenderer,\
	SDL_Window **gWindow);

void close(SDL_Renderer **gRenderer, SDL_Window **gWindow);

void read_characters(LTexture font, std::map<char, SDL_Rect> *letterClips);

void console_print(SDL_Renderer *renderer, LTexture *font, std::map<char, SDL_Rect> *letterClips,\
	const char* str, int *row, int *column, SDL_Rect *viewport, int SCREEN_WIDTH,\
	int SCREEN_HEIGHT);

std::vector<SDL_Rect> generateRects(MatrixGraph maze, int SCREEN_WIDTH,\
	int SCREEN_HEIGHT, int STROKE);
#endif