#include "Map.h"
#include "mbed.h"
#include "arduino.h"
#include "Map.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"
#include "MotionController.h"

#define MAZE_RESOLUTION 120
#define X 0
#define Y 1
#define GRID_X_MAX 12
#define GRID_Y_MAX 16
#define STARTING_X 3
#define STARTING_Y 14
#define DEFAULT_BRIGHTNESS 10

// arrays to store coordinates of robots current and previous location
int previousLocation[2];
int currentLocation[2];

int angle;

// create 2D array to represent the maze
const char *occupancyGrid[GRID_X_MAX][GRID_Y_MAX];

using namespace mbed;

// build a matrix object with the same dimensions as my physical LED matrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, MAP_PIN,
                                               NEO_TILE_BOTTOM + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                                                   NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_BGR + NEO_KHZ800);

void mapSetup()
{
    // set up default matrix parameters
    matrix.begin();
    matrix.setBrightness(DEFAULT_BRIGHTNESS);
    // curser starts in top left corner of matrix
    matrix.setCursor(0, 0);
    mapReset();
    // command to refresh the matrix with any changes made
    matrix.show();
}

void mapReset()
{
    // turn all LEDs off
    matrix.fillScreen(OFF);
    resetGrid();
    // Draw rectangle with relative dimensions of the maze
    mapDrawBoundary();
    // set starting robot location to bottom left of maze within bounds
    previousLocation[X] = STARTING_X;
    previousLocation[Y] = STARTING_Y;
    currentLocation[X] = STARTING_X;
    currentLocation[Y] = STARTING_Y;
    angle = 0;
    // draw robot onto the matrix
    matrix.drawPixel(currentLocation[X], currentLocation[Y], BLUE);
    matrix.drawPixel(currentLocation[X], currentLocation[Y] - 1, PURPLE);
    matrix.show();
}

void mapOverwriteLocation(int x, int y, int angle)
{
    currentLocation[X] = x;
    currentLocation[Y] = y;
    angle = angle;
    mapUpdate();
}

void mapUpdate(void)
{
    // draws a line on the LED screen to show the route taken
    matrix.drawLine(previousLocation[X], previousLocation[Y], currentLocation[X], currentLocation[Y], RED);
    // draws a pixel on the screen to show the new location
    matrix.drawPixel(currentLocation[X], currentLocation[Y], BLUE);
    matrix.show();

    // marks the route taken as free space in the occupancy grid
    for (int y = currentLocation[Y]; y <= previousLocation[Y]; y++)
    {
        updateGrid(currentLocation[X], y, "O");
    }

    angle = motorControl.getCurrentAngle();
    if (angle == 0)
    {
        matrix.drawPixel(currentLocation[X], currentLocation[Y] - 1, PURPLE);
    }
    else if (angle == 90)
    {
        matrix.drawPixel(currentLocation[X] + 1, currentLocation[Y], PURPLE);
    }
    else if (angle == 180)
    {
        matrix.drawPixel(currentLocation[X], currentLocation[Y] + 1, PURPLE);
    }
    else if (angle == 270)
    {
        matrix.drawPixel(currentLocation[X] - 1, currentLocation[Y], PURPLE);
    }
    else
    {
    }
}

void mapUpdateLocation(int distance)
{
    // get the current angle the robot is facting (0 = towards finish, increasing clockwise to 360)
    angle = motorControl.getCurrentAngle();

    if (angle == 0)
    {
        currentLocation[Y] -= distance / MAZE_RESOLUTION;
    }
    else if (angle == 90)
    {
        currentLocation[X] += distance / MAZE_RESOLUTION;
    }
    else if (angle == 180)
    {
        currentLocation[Y] += distance / MAZE_RESOLUTION;
    }
    else if (angle == 270)
    {
        currentLocation[X] -= distance / MAZE_RESOLUTION;
    }
    else
    {
    }
    mapUpdate();
    previousLocation[X] = currentLocation[X];
    previousLocation[Y] = currentLocation[Y];
}

int getCurrentX(void)
{
    return currentLocation[Y];
}
int getCurrentY(void)
{
    return currentLocation[X];
}

void errorScreen(void)
{
    matrix.fillScreen(RED);
}

void drawObstacle(void)
{
    matrix.drawPixel(currentLocation[X], currentLocation[Y] - 1, YELLOW);
    matrix.show();

    updateGrid(currentLocation[X], currentLocation[Y] - 1, "X");
}

void resetGrid(void)
{
    for (int y = 0; y < GRID_Y_MAX; y++)
    {
        for (int x = 0; x < GRID_X_MAX; x++)
        {
            occupancyGrid[x][y] = "?";
        }
    }
}

void updateGrid(int x, int y, const char *object)
{
    occupancyGrid[x][y] = object;
}

void printGrid()
{
    for (int y = 0; y < GRID_Y_MAX; y++)
    {
        for (int x = 0; x < GRID_X_MAX; x++)
        {
            Serial.print(occupancyGrid[x][y]);
        }
        Serial.println("");
    }
}

void mapDrawBoundary()
{
    matrix.drawRect(2, 0, GRID_X_MAX, GRID_Y_MAX, YELLOW);
}