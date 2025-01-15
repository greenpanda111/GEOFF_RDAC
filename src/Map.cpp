#include "Map.h"
#include "mbed.h"
#include "Map.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"
#include "MotionController.h"

// Resolution in mm/pixel
#define MAZE_RESOLUTION 120
// Array addresses for coordinate location
#define X 0
#define Y 1
// Max size of occupancy grid
#define GRID_X_MAX 12
#define GRID_Y_MAX 16
// Starting coordinates of robot in matrix
#define STARTING_X 3
#define STARTING_Y 14
// LED brightness %
#define DEFAULT_BRIGHTNESS 10

// Matrix is made of 4 tiles in a 2x2 orientation, each with a grid of 8x8 LEDs in a square to create a 16x16 matrix of LEDs
#define TILE_LED_WIDTH 8
#define TILE_LED_HEIGHT 8
#define TILES_WIDE 2
#define TILES_HIGH 2

// Arrays to store coordinates of the current and previous location of the robot
int previousLocation[2];
int currentLocation[2];
// Current angle the robot is facing (0 = finish, clockwise increases)
int angle;

// Create 2D array to represent the maze
const char *occupancyGrid[GRID_X_MAX][GRID_Y_MAX];

using namespace mbed;

// Build a matrix object
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(TILE_LED_WIDTH, TILE_LED_HEIGHT, TILES_WIDE, TILES_HIGH, MAP_PIN,
                                               NEO_TILE_BOTTOM + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                                                   NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_BGR + NEO_KHZ800);

void mapSetup()
{
    // Set up default matrix parameters
    matrix.begin();
    matrix.setBrightness(DEFAULT_BRIGHTNESS);
    // Curser starts in top left corner of matrix
    matrix.setCursor(0, 0);
    mapReset();
    // Command to refresh the matrix with any changes made
    matrix.show();
}

void mapReset()
{
    // Turn all LEDs off
    matrix.fillScreen(OFF);
    // Fill occupancy grid with "?" to represent unknown
    resetGrid();
    // Draw rectangle with relative dimensions of the maze
    mapDrawBoundary();
    // Set starting robot location to bottom left of maze within bounds, facing towards the finish
    previousLocation[X] = STARTING_X;
    previousLocation[Y] = STARTING_Y;
    currentLocation[X] = STARTING_X;
    currentLocation[Y] = STARTING_Y;
    angle = 0;
    // Draw robot onto the matrix
    matrix.drawPixel(currentLocation[X], currentLocation[Y], BLUE);
    matrix.drawPixel(currentLocation[X], currentLocation[Y] - 1, PURPLE);
    matrix.show();
}

void mapUpdate(void)
{
    // Draws a line on the LED screen to show the route taken
    matrix.drawLine(previousLocation[X], previousLocation[Y], currentLocation[X], currentLocation[Y], RED);
    // Draws a pixel on the screen to show the new location
    matrix.drawPixel(currentLocation[X], currentLocation[Y], BLUE);
    matrix.show();

    // Marks the route taken as free space in the occupancy grid
    for (int y = currentLocation[Y]; y <= previousLocation[Y]; y++)
    {
        updateGrid(currentLocation[X], y, "O");
    }

    // Dispalys the heading of the robot with a purple pixel in the direction it is facing
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
}

void mapUpdateLocation(int distance)
{
    // Get the current angle the robot is facting (0 = towards finish, increasing clockwise to 360)
    angle = motorControl.getCurrentAngle();

    // If facing finish
    if (angle == 0)
    {
        // Current location moves towards finish
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

    // Draw new location onto matrix and update occupancy grid
    mapUpdate();
    // Update previous location
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
    // Draws a yellow pixel ahead of the robot
    matrix.drawPixel(currentLocation[X], currentLocation[Y] - 1, YELLOW);
    matrix.show();
    // Sets the position ahead of the robot as an obstacle in the occupancy grid
    updateGrid(currentLocation[X], currentLocation[Y] - 1, "X");
}

void resetGrid(void)
{
    // Loop through 2D array
    for (int y = 0; y < GRID_Y_MAX; y++)
    {
        for (int x = 0; x < GRID_X_MAX; x++)
        {
            // Set all postitions to "?"
            occupancyGrid[x][y] = "?";
        }
    }
}

void updateGrid(int x, int y, const char *object)
{
    // Take position and character and enter it into the 2D array
    occupancyGrid[x][y] = object;
}

void printGrid()
{
    // Loop through 2D array
    for (int y = 0; y < GRID_Y_MAX; y++)
    {
        for (int x = 0; x < GRID_X_MAX; x++)
        {
            // Print every value
            Serial.print(occupancyGrid[x][y]);
        }
        // Create a new line after each line of x values has been printed
        Serial.println("");
    }
}

void mapDrawBoundary()
{
    // Draw a rectangle to represent the maze boundary
    matrix.drawRect(2, 0, GRID_X_MAX, GRID_Y_MAX, YELLOW);
}