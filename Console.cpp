/*
	Author: Nathan Smith

	This file contains the functions defining behavior of the console, a display of text
	in the program.
	It operated by reading parts of a png file and rendering those clips
	ASCII characters are first read into a map, which maps teh character codes to the clip
*/

#include <SDL.h>
#include <SDL/SDL_image.h>
#include "prototypes.h"

void read_characters(LTexture font, std::map<char, SDL_Rect> *letterClips) {
	int row, column, i;

	//Row 2, column 0 is the starting point
	row = 2;
	column = 0;

	//Start reading at the ASCII code for ' '
	i = 32;

	//Stopping at row 8, column 0
	while (row < 8) {
		//Get the cropped letter
		SDL_Rect temp = { 64 * column, 64 * row, 64, 64 };

		//map the character to its part of the spritesheet
		letterClips->insert(std::pair<char, SDL_Rect>(i, temp));
		
		//increment the character code, and the column
		i++;
		column++;

		//If at the end of the row, increment row, reset column
		if (column > 15) {
			row++;
			column = 0;
		}
	}
}

void console_print(SDL_Renderer *gRenderer, LTexture *font, std::map<char, SDL_Rect> *letterClips, \
	const char* str, int *row, int *column, SDL_Rect *viewport, int SCREEN_WIDTH, \
	int SCREEN_HEIGHT) {

	int i = 0;

	//Set the location of the first character to render
	SDL_Rect charView = { viewport->x + (*column) * viewport->w / 15, viewport->y + (*row ) * viewport->h / 48, 16, 16 };
	SDL_RenderSetViewport(gRenderer, &charView);

	//Render the string
	while (str[i] != '\0') {
		if (str[i] != '\n') {
			font->render(0, 0, &(letterClips->at(str[i])));
			*column += 1;
			charView.x += 16;
		}
		if (str[i] == '\n' || (*column) > 14) {
			*row += 1;
			charView.y += 16;
			*column = 0;
			charView.x = viewport->x + (*column) * viewport->x / 30;
		}
		i++;
		SDL_RenderSetViewport(gRenderer, &charView);
	}
}
