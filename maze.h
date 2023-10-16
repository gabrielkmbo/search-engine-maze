#pragma once

#include "grid.h"
#include "set.h"
#include <string>

// Prototypes to be shared with other modules

Set<GridLocation> generateValidMoves(Grid<bool>& g, GridLocation cur);

void validatePath(Grid<bool>& g, Vector<GridLocation>& path);

void readMazeFile(std::string filename, Grid<bool>& maze);

void readSolutionFile(std::string filename, Vector<GridLocation>& soln);

bool solveMazeBFS(Grid<bool>& maze, Vector<GridLocation>& soln);

bool solveMazeDFS(Grid<bool>& maze, Vector<GridLocation>& soln);
