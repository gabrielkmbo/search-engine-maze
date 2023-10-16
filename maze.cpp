/*
 * Maze Solving and Validation Functions
 *
 * This file contains functions for generating valid moves in a maze,
 * validating a given path for solving the maze, and solving a maze using
 * both breadth-first search (BFS) and depth-first search (DFS) algorithms.
 *
 * @author Gabriel Bo
 * course: CS106B
 * date: 10/16/2023
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "SimpleTest.h"
using namespace std;


/*
 * The generateValidMoves function is given a maze and cur argument.
 * Using the maze given and the current location on the grid, this function
 * puts the grid location of north, east, south, and west into a vector
 * and tests whether each of the moves are possible: it's a "true" space
 * or if the space exists.
 * @param maze is the inputed maze with true or false coordinates that the user is trying
 * to solve
 * @param cur is the location placed to test whether its north, east, south, west neighbors
 * are viable moves
 * @return a set of 0, 1, 2, 3, or 4 locations that the next move could be placed in
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    Vector<GridLocation> moves = {
        GridLocation(cur.row - 1, cur.col), //North
        GridLocation(cur.row + 1, cur.col), //South
        GridLocation(cur.row, cur.col + 1), //East
        GridLocation(cur.row, cur.col - 1)  //West
    };

    // Testing whether each move is open or a "true move"
    for(GridLocation &move : moves)
    {
        if(maze.inBounds(move.row, move.col) && maze[move.row][move.col] == true )
        {
            neighbors.add(move);
        }
    }

    return neighbors;
}

/*
 * The validatePath function takes in a maze and path and tells
 * whether or not the coordinated path can correctly solve the maze.
 * @param maze is the puzzle needing to be solved
 * @param is a vector that lists out coordinates that attempts to solve the maze
 * @return an error statement if the maze cannot be solved and why it can't be solved.
 */
void validatePath(Grid<bool>& maze, Vector<GridLocation>& path) {
    if (path.isEmpty())
    {
        error("Path is empty!");
    }

    GridLocation start = {0, 0};
    GridLocation exit = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation> beenThere;

    if(path[0] != start)
    {
        error("Path doesn't begin at the entry (upper left corner)");
    }
    if(path[path.size() - 1] != exit)
    {
        error("Path doesn't end at the exit (lower right corner)");
    }
    for(int i = 0; i < path.size() - 1; i++)
    {
        if (!generateValidMoves(maze, path[i]).contains(path[i + 1]))
        {
            error("Path doesn't contain valid moves");
        }
        else if (beenThere.contains(path[i]))
        {
            error("Path contains a looped sequence");
        }
        else
        {
            beenThere.add(path[i]);
        }
    }


    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without raising any errors.
     */
}

/*
 * The solveMazeBFS function takes in a parameter maze and soln.
 * This function uses breadth first search to test the paths in the maze.
 * Using the generateValidMoves function, solveMazeBFS can generate several
 * paths and test whether or not the maze can be solved and reach the exit.
 * If it can be solved it returns true and assigns the soln variable the solution
 * vector to the maze. Similar to solveMazeDFS, but instead of using stacks it uses queues.
 * @param maze is the maze that needs to be solved
 * @param soln is the variable used to hold the solutions to the maze if generated
 * @return true if the maze can be solved and false if it is empty or can't be solved
 */
bool solveMazeBFS(Grid<bool>& maze, Vector<GridLocation>& soln) {
    Queue<Vector<GridLocation>> allPaths;
    Set<GridLocation> visited;
    allPaths.enqueue({{0, 0}});
    visited.add({0, 0});
    GridLocation currentLocation = {0, 0};

    while(!allPaths.isEmpty())
    {
        //setting up a test for a new set path
        Vector<GridLocation> currentPath = allPaths.peek();
        allPaths.dequeue();

        currentLocation = currentPath.get(currentPath.size() - 1);

        //testing whether the current location is the exit of the maze
        if(currentLocation.row == maze.numRows() - 1 && currentLocation.col == maze.numCols() - 1)
        {
            soln = currentPath;
            return true;
        }

        Set<GridLocation> potentialMoves = generateValidMoves(maze, currentLocation);
        for(auto nextMove : potentialMoves)
        {
            GridLocation nextLocation = {nextMove.row, nextMove.col};
            if(!visited.contains({nextMove.row, nextMove.col}))
            {
                visited.add({nextMove.row, nextMove.col});
                Vector<GridLocation> newPath = currentPath;
                newPath.add(nextLocation);
                allPaths.enqueue(newPath);
            }
        }
    }

    return false;
}

/*
 * The solveMazeDFS function takes in a parameter maze and soln.
 * This function uses depth first search to test the paths in the maze.
 * Using the generateValidMoves function, solveMazeDFS can generate several
 * paths and test whether or not the maze can be solved and reach the exit.
 * If it can be solved it returns true and assigns the soln variable the solution
 * vector to the maze. Similar to solveMazeBFS, but instead of using queues it uses stacks.
 * @param maze is the maze that needs to be solved
 * @param soln is the variable used to hold the solutions to the maze if generated
 * @return true if the maze can be solved and false if it is empty or can't be solved
 */
bool solveMazeDFS(Grid<bool>& maze, Vector<GridLocation>& soln) {
    Stack<Vector<GridLocation>> allPaths;
    Set<GridLocation> visited;
    allPaths.push({{0, 0}});
    visited.add({0, 0});
    GridLocation currentLocation = {0, 0};

    while(!allPaths.isEmpty())
    {
        //setting up a test for a new set path
        Vector<GridLocation> currentPath = allPaths.peek();
        allPaths.pop();

        currentLocation = currentPath.get(currentPath.size() - 1);

        //testing whether the current location is the exit of the maze
        if(currentLocation.row == maze.numRows() - 1 && currentLocation.col == maze.numCols() - 1)
        {
            soln = currentPath;
            return true;
        }

        Set<GridLocation> potentialMoves = generateValidMoves(maze, currentLocation);
        for(auto nextMove : potentialMoves)
        {
            GridLocation nextLocation = {nextMove.row, nextMove.col};
            if(!visited.contains({nextMove.row, nextMove.col}))
            {
                visited.add({nextMove.row, nextMove.col});
                Vector<GridLocation> newPath = currentPath;
                newPath.add(nextLocation);
                allPaths.push(newPath);
            }
        }
    }

    return false;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines = readLines(in);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
    if (!maze[0][0] || !maze[numRows-1][numCols-1]) {
        error("Maze entrance and exit must be both be open corridors");
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Vector<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

STUDENT_TEST("sovleMazeDFS on hand-constructed maze")
{
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Vector<GridLocation> soln;
    EXPECT(solveMazeDFS(maze, soln));
    EXPECT_NO_ERROR(validatePath(maze, soln));

    Grid<bool> maze1 = {{true, false, true, false},
                        {true, true, false, false},
                        {false, true, true, false},
                        {false, false, true, true}};

    Vector<GridLocation> soln1 = {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}, {3, 2}, {3, 3}};
    Vector<GridLocation> solnGenerated;
    EXPECT(solveMazeDFS(maze1, solnGenerated));
    EXPECT_EQUAL(solnGenerated, soln1);
    EXPECT_NO_ERROR(validatePath(maze1, solnGenerated));
}


STUDENT_TEST("sovleMazeBFS on hand-constructed maze")
{
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Vector<GridLocation> soln;
    EXPECT(solveMazeBFS(maze, soln));
    EXPECT_NO_ERROR(validatePath(maze, soln));

    Grid<bool> maze1 = {{true, false, true, false},
                       {true, true, false, false},
                       {false, true, true, false},
                       {false, false, true, true}};

    Vector<GridLocation> soln1 = {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}, {3, 2}, {3, 3}};
    Vector<GridLocation> solnGenerated;
    EXPECT(solveMazeBFS(maze1, solnGenerated));
    EXPECT_EQUAL(solnGenerated, soln1);
    EXPECT_NO_ERROR(validatePath(maze1, solnGenerated));
}


STUDENT_TEST("validatePath on correct solution, hand-constructed 3x3 maze")
{
    Grid<bool> maze = {{true, false, false},
                       {true, false, false},
                       {true, true, true}};
    Vector<GridLocation> soln = {{0 ,0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    EXPECT_NO_ERROR(validatePath(maze, soln));

    Vector<GridLocation> loop = {{0 ,0}, {1, 0}, {2, 0}, {2, 1}, {2, 0}, {2, 2}};
    Vector<GridLocation> moveOutOfBounds = {{0 ,0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}, {3, 2}, {2, 2}};
    Vector<GridLocation> nonValidMove = {{0 ,0}, {1, 0}, {2, 0}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, loop));
    EXPECT_ERROR(validatePath(maze, moveOutOfBounds));
    EXPECT_ERROR(validatePath(maze, nonValidMove));
}

STUDENT_TEST("generateValidMoves accesses only one \"step\" away from the current location")
{
    Grid<bool> maze = {{false, true, false},
                       {true, false, true},
                       {false, false, false}};
    GridLocation space = {0, 1};
    Set<GridLocation> expected = {};
    EXPECT_EQUAL(generateValidMoves(maze, space), expected);

    space = {2, 2};
    expected = {{1, 2}};
    EXPECT_EQUAL(generateValidMoves(maze, space), expected);

    space = {1, 1};
    expected = {{0, 1}, {1, 0}, {1, 2}};
    EXPECT_EQUAL(generateValidMoves(maze, space), expected);
}

/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution, hand-constructed maze") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Vector<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution read from file, medium maze") {
    Grid<bool> maze;
    Vector<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true},
                       {true, true}};
    Vector<GridLocation> notBeginAtEntry = { {1, 1}, {2, 1} };
    Vector<GridLocation> notEndAtExit = { {0, 0}, {1, 0}, {2, 0} };
    Vector<GridLocation> moveThroughWall = { {0 ,0}, {0, 1}, {1, 1}, {2, 1} };

    EXPECT_ERROR(validatePath(maze, notBeginAtEntry));
    EXPECT_ERROR(validatePath(maze, notEndAtExit));
    EXPECT_ERROR(validatePath(maze, moveThroughWall));
}

PROVIDED_TEST("solveMazeBFS on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Vector<GridLocation> soln;
//    initGraphicsForMaze(maze, true); //optional, you can remove this line if you do not wish to animate
    EXPECT(solveMazeBFS(maze, soln));
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMazeDFS on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Vector<GridLocation> soln;
    initGraphicsForMaze(maze, true); //optional, you can remove this line if you do not wish to animate
    EXPECT(solveMazeDFS(maze, soln));
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("Attempt to solve maze with no solution") {
    Grid<bool> maze(4, 4, false); // all grid elements initialized to false (walls)
    maze[0][0] = maze[3][3] = true; // set entrance and exit to true (open corridors)
    Vector<GridLocation> soln;
    EXPECT(!solveMazeBFS(maze, soln));
    EXPECT(!solveMazeDFS(maze, soln));
}
