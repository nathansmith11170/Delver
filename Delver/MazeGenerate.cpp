#include "MatrixGraph.h"
#include "prototypes.h"
#include <stack>
#include <algorithm>
#include <ctime>
#include <cstdlib>

static int recursiveBacktracker(MatrixGraph grid, MatrixGraph *maze, \
	std::stack<int> *stack, std::unordered_set<int> *visitedSet, \
	int currentVertex) {
	int i, j, isVisited = 0, randomIndex, nextVertex;
	std::vector<int> unvisitedNeighbors;
	std::vector<int> neighbors;
	int isvisited;

	//From the neighbors of the current node, determine which are unvisited
	neighbors = grid.get_neighbors(currentVertex);
	for (i = 0; i < neighbors.size(); i++) {
		if (!(*visitedSet).count(neighbors.at(i))) {
			unvisitedNeighbors.push_back(neighbors.at(i));
		}
	}

	if (!unvisitedNeighbors.empty()) {
		//Get a random neighbor
		std::random_shuffle(unvisitedNeighbors.begin(), unvisitedNeighbors.end());
		nextVertex = unvisitedNeighbors.back();
		//Push current node onto the stack
		(*stack).push(currentVertex);
		//Add the appropriate edge to the maze
		(*maze).add_edge(currentVertex, nextVertex);
		(*visitedSet).insert(nextVertex);
	}
	else if (!(*stack).empty()) {
		//If there were no unvisited neighbors, go to the next vertex in the frontier
		nextVertex = (*stack).top();
		(*stack).pop();
	}
	return nextVertex;
}

MatrixGraph mazeGenerate(int NODES) {
	srand(time(NULL));
	//Define two graphs
	MatrixGraph maze, grid;

	//The size of the maze
	int i, row, column;

	//Data for maze generation
	std::stack<int> frontier;
	std::unordered_set<int> visited;
	int currentVertex = 0;

	//Add the vertices to the graph
	for (i = 0; i < NODES * NODES; i++) {
		maze.add_vertice(i);
		grid.add_vertice(i);
	}

	//Populate the grid with the proper edges
	row = 0;
	column = 0;
	for (i = 0; i < NODES * NODES; i++) {
		if (i + 1 <= (row + 1) * NODES - 1) {
			grid.add_edge(i, i + 1);
		}
		if (i - 1 >= row * NODES) {
			grid.add_edge(i, i - 1);
		}
		if (i + NODES <= NODES * NODES - 1) {
			grid.add_edge(i, i + NODES);
		}
		if (i - NODES >= 0) {
			grid.add_edge(i, i - NODES);
		}

		column++;
		if (column >= NODES) {
			column = 0;
			row++;
		}
	}

	//Generate a maze
	frontier.push(currentVertex);
	while (!frontier.empty()) {
		currentVertex = recursiveBacktracker(grid, &maze, &frontier, &visited, currentVertex);
	}

	//Clear the unused grid
	grid.clear();
	return maze;
}

std::vector<SDL_Rect> generateRects( MatrixGraph maze, int SCREEN_WIDTH, int SCREEN_HEIGHT, int STROKE ) {
	//Set up vertices
	int row = 0, column = 0, i, j;
	long columns = (long)sqrtl(maze.size());
	std::vector<SDL_Rect> rects;
	for (i = 0; i < maze.size(); i++) {
		SDL_Rect temp = {
			STROKE + column * ((SCREEN_WIDTH - 240) / columns),
			STROKE + row * (SCREEN_HEIGHT / columns),
			(SCREEN_WIDTH - 240) / columns - 2 * STROKE,
			SCREEN_HEIGHT / columns - 2 * STROKE
		};

		rects.push_back(temp);

		column++;
		if (column >= sqrt(maze.size())) {
			column = 0;
			row++;
		}
	}

	//Set up edges
	for (i = 0; i < maze.size(); i++) {
		std::vector<int> temp = maze.get_neighbors(i);
		for (j = 0; j < temp.size(); j++) {
			if (temp.at(j) == i + 1) {
				rects.at(i).w += 2 * STROKE;
			}
			else if (temp.at(j) == i + columns) {
				rects.at(i).h += 2 * STROKE;
			}
		}
	}
	return rects;
}