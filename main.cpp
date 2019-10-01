/*
	Author: Nathan Smith

	This file holds the program logic and the references to the game's data.
	It handles user input and display.
*/

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "MatrixGraph.h"
#include "prototypes.h"
#include <ctime>
#include <map>
#include <set>
#include <string>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define NODES 20
#define STROKE 2

void close(SDL_Renderer **renderer, SDL_Window **window) {
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	*renderer = NULL;
	*window = NULL;

	SDL_Quit();
}

int main(int argc, char* argv[]) {
	int currentVertex;
	long long score;
	std::vector<SDL_Rect> rects;

	//Name the window we'll be rendering to
	SDL_Window *gWindow = NULL;

	//SDL renderer
	SDL_Renderer *gRenderer = NULL;

	//Generate a maze with NODES * NODES vertices 
	MatrixGraph maze = mazeGenerate(20);
	std::set<int> visitedSet;

	//Console viewport
	SDL_Rect console = { SCREEN_WIDTH - 240, 0, 240, SCREEN_HEIGHT };

	//Borders
	SDL_Rect topBorder = { STROKE, 0, SCREEN_WIDTH, STROKE };
	SDL_Rect leftBorder = { 0, 0, STROKE, SCREEN_HEIGHT };

	rects = generateRects(maze, SCREEN_WIDTH, SCREEN_HEIGHT, STROKE);
	
	//Set current vertex to 0
	currentVertex = 0;
	score = 0;


	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, &gRenderer, &gWindow) == 0) {
		printf("Failed to initialize.");
	}
	else
	{
		int row, column, i, j;
		LTexture fontTexture = LTexture( gRenderer );
		LTexture tileSet = LTexture( gRenderer );

		std::map<char, SDL_Rect> letterClips;
		read_characters(fontTexture, &letterClips);

		//Load fontTexture
		fontTexture.loadFromFile("font.png");

		//Load tileset
		tileSet.loadFromFile("tileset.png");

		//Console
		SDL_Rect consoleView = { SCREEN_WIDTH - 240, 0, 240, SCREEN_HEIGHT };

		SDL_Event e;
		bool quit = false;
		int spriteFrame = 0, frameDelay = 0;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_KEYUP) {
					if (e.key.keysym.sym == SDLK_RIGHT) {
						std::vector<int> temp = maze.get_neighbors(currentVertex);
						for (i = 0; i < temp.size(); i++) {
							if (currentVertex + 1 == temp.at(i)) {
								if (visitedSet.count(currentVertex + 1) == 0) {
									score += 10;
								}
								currentVertex += 1;
								visitedSet.insert(currentVertex);
								score -= 1;
							}
						}
					}
					else if (e.key.keysym.sym == SDLK_LEFT) {
						std::vector<int> temp = maze.get_neighbors(currentVertex);
						for (i = 0; i < temp.size(); i++) {
							if (currentVertex - 1 == temp.at(i)) {
								if (visitedSet.count(currentVertex - 1) == 0) {
									score += 10;
								}
								currentVertex -= 1;
								visitedSet.insert(currentVertex);
								score -= 1;
							}
						}
					}
					else if (e.key.keysym.sym == SDLK_DOWN) {
						if (currentVertex == NODES * NODES - 1) {
							//RECREATE MAZE
							maze = mazeGenerate(20);
							//SET currentVertex back to 0
							currentVertex = 0;
							//CLEAR visitedSet
							visitedSet.clear();
							//Reset edges
							rects.clear();
							rects = generateRects(maze, SCREEN_WIDTH, SCREEN_HEIGHT, STROKE);
							continue;
						}
						std::vector<int> temp = maze.get_neighbors(currentVertex);
						for (i = 0; i < temp.size(); i++) {
							if (currentVertex + NODES == temp.at(i)) {
								if (visitedSet.count(currentVertex + NODES) == 0) {
									score += 10;
								}
								currentVertex += NODES;
								visitedSet.insert(currentVertex);
								score -= 1;
							}
						}
					}
					else if (e.key.keysym.sym == SDLK_UP) {
						std::vector<int> temp = maze.get_neighbors(currentVertex);
						for (i = 0; i < temp.size(); i++) {
							if (currentVertex - NODES == temp.at(i)) {
								if (visitedSet.count(currentVertex -NODES) == 0) {
									score += 10;
								}
								currentVertex -= NODES;
								visitedSet.insert(currentVertex);
								score -= 1;
							}
						}
					}
				}
			}
			//Frame Logic
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(gRenderer);

			//Draw adjacent tiles
			SDL_RenderSetViewport(gRenderer, NULL);
			std::vector<int> temp = maze.get_neighbors(currentVertex);
			SDL_Rect clip;
			for (j = 0; j < temp.size(); j++) {
				SDL_RenderSetViewport(gRenderer, &rects.at(temp.at(j)));
				clip = { 16, 64, 16, 16 };
				tileSet.render(0, 0, &clip);
				if (visitedSet.count(temp.at(j)) == 0) {
					switch (spriteFrame) {
						case 0:
							clip = { 288, 272, 8, 9 };
							break;
						case 1:
							clip = { 296, 272, 8, 9 };
							break;
						case 2:
							clip = { 304, 272, 8, 9 };
							break;
						case 3:
							clip = { 312, 272, 8, 9 };
							break;
					}
					tileSet.render(0, 0, &clip);
				}
				SDL_RenderSetViewport(gRenderer, NULL);
			}

			//Draw exit
			i = NODES * NODES - 1;
			SDL_RenderSetViewport(gRenderer, &rects.at(i));
			clip = { 48, 96, 16, 16 };
			tileSet.render(0, 0, &clip);
			SDL_RenderSetViewport(gRenderer, NULL);

			//Draw current tile and avatar
			i = currentVertex;
			SDL_RenderSetViewport(gRenderer, &rects.at(i));
			clip = { 16, 64, 16, 16 };
			tileSet.render(0, 0, &clip);
			switch (spriteFrame) {
			case 0:
				clip = { 128, 44, 16, 18 };
				break;
			case 1:
				clip = { 144, 44, 16, 18 };
				break;
			case 2:
				clip = { 160, 44, 16, 18 };
				break;
			case 3:
				clip = { 176, 44, 16, 18 };
				break;
			}

			//Slow down the animation by counting ten frames before changing 
			//sprite.
			frameDelay++;
			if (frameDelay > 10) {
				spriteFrame++;
				frameDelay = 0;
				if (spriteFrame > 3) {
					spriteFrame = 0;
				}
			}

			tileSet.render(0, 0, &clip);

			//Console
			row = 0;
			column = 0;
			console_print(gRenderer, &fontTexture, &letterClips, "The dark,\n", &row,\
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "twisting\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "corridors of\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "the maze seem\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "to press in\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "around you.\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "The maze goes:\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			
			for (i = 0; i < temp.size(); i++) {
				if (temp.at(i) == currentVertex - NODES) {
					console_print(gRenderer, &fontTexture, &letterClips, "north\n", &row, \
						&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				if (temp.at(i) == currentVertex + NODES) {
					console_print(gRenderer, &fontTexture, &letterClips, "south\n", &row, \
						&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				if (temp.at(i) == currentVertex - 1) {
					console_print(gRenderer, &fontTexture, &letterClips, "west\n", &row, \
						&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				if (temp.at(i) == currentVertex + 1) {
					console_print(gRenderer, &fontTexture, &letterClips, "east\n", &row, \
						&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
			}

			if (currentVertex == NODES * NODES - 1) {
				console_print(gRenderer, &fontTexture, &letterClips, "There is a\n", &row, \
					&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				console_print(gRenderer, &fontTexture, &letterClips, "ladder going\n", &row, \
					&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				console_print(gRenderer, &fontTexture, &letterClips, "down. Press\n", &row, \
					&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				console_print(gRenderer, &fontTexture, &letterClips, "the down\n", &row, \
					&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
				console_print(gRenderer, &fontTexture, &letterClips, "arrow.\n", &row, \
					&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			}

			std::string str = std::to_string(score);
			console_print(gRenderer, &fontTexture, &letterClips, "\nScore:", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, str.c_str(), &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);
			console_print(gRenderer, &fontTexture, &letterClips, "\n", &row, \
				&column, &consoleView, SCREEN_WIDTH, SCREEN_HEIGHT);

			SDL_RenderPresent(gRenderer);
		}
	}

	close(&gRenderer, &gWindow);
	return 0;
}
