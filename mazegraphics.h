/**
 * File: mazegraphics.h
 * --------------------
 * @author Julie Zelenski, Fall 2023
 * Defines functions to draw a maze in graphics window.
 */

#pragma once
#include <string>
#include "grid.h"


/**
 * The initGraphicsForMaze function takes a Grid parameter representing
 * a maze.  You should call initGraphicsForMaze once before starting to solve the
 * maze to set up the animation. The animation window which display the maze
 * as a grid of corridors and walls.  A grid location that is true is
 * an open corridor (drawn in white), false is a wall (drawn in dark gray).
 *
 * The optional pauseForClick parameter allows you to configure the
 * animation to pause on each call to highlightPath and require a click from
 * the user before proceeding. If not specified, pauseForClick defaults to false,
 * indicating the animation should proceed without pausing.
 */
void initGraphicsForMaze(const Grid<bool>& maze, bool pauseForClick = false);

/**
 * The highlightPath function takes a path (Vector of GridLocation) and
 * marks the path on the maze by drawing a filled dot on each location.
 * The fill color of the dots is set by the color parameter.  Color can
 * be a color name such as "red", "blue", "green" or a hex color string "#33ff33"
 *
 * highlightPath expects to mark path locations on top of an already
 * drawn maze. A call to highlightPath without having previously
 * called initialized the graphics is ignored. This is a convenience that allows
 * you to selectively decide whether a solve operation animates or not. If the
 * graphics were initialized with pauseForClick = true, each call to highlightPath
 * will pause and wait for a click from user before proceeding.
 */
void highlightPath(const Vector<GridLocation>& p, std::string color);


/**
 * The printMaze function can optionally be used to output a text version
 * of the maze for debugging or accessbility.
 * First call initGraphicsForMaze and highlightPath to draw maze in graphics window, then
 * follow with a call to printMaze to output the corresponding text version.
 * In the text form, the character '@' is used for wall, space for corridor,
 * and '+' marks highlighted locations on the path.
 *
 * Example output:
 *                     +  @  @     @  @
 *                     +  +  +  +  +  @
 *                     @  @  @  @  +  @
 *                                 +  +
 */
void printMaze();

