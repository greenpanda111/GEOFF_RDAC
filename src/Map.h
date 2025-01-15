#ifndef MAP_H
#define MAP_H

#include "mbed.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"

// "Grid" is used in reference to the occupancy grid, internally stored
// "Matrix" is used in reference to the external LED panel
// "Map" is used to reference both the occupancy grid and LED matrix where appropriate

// Signal pin for the LED map
#define MAP_PIN A7

// List of basic colours for easy reference when changing the matrix
#define OFF matrix.Color(0, 0, 0)
#define WHITE matrix.Color(255, 255, 255)
#define RED matrix.Color(0, 255, 0)
#define GREEN matrix.Color(0, 0, 255)
#define BLUE matrix.Color(255, 0, 0)
#define YELLOW matrix.Color(0, 255, 255)
#define PURPLE matrix.Color(255, 255, 0)

using namespace mbed;

// Initialise basic parameters of the matrix and grid
void mapSetup(void);
// Empty the map and reset the starting position of the robot
void mapReset(void);
// Draws the path travelled and the current position and heading of the robot
void mapUpdate(void);
// Draws approximation of the maze outline onto the matrix
void mapDrawBoundary(void);
// Updates the internally stored current and previous location of the robot
void mapUpdateLocation(int distance);
// Return location of the robot
int getCurrentX(void);
int getCurrentY(void);

// Fill matrix with red
void errorScreen(void);
// Draw Yellow pixel ahead of the current location of the robot
void drawObstacle(void);
// Fil the occupancy grid with "?"
void resetGrid(void);
// Fill an entry in the occupancy grid with a character of choice at a specific location
void updateGrid(int x, int y, const char *object);
// Prints grid to serial
void printGrid();

#endif // MAP_H
