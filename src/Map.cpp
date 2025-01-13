#include "Map.h"
#include "mbed.h"
#include "arduino.h"
#include "Map.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"
#include "MotionController.h"

int previousLocation[2];
int currentLocation[2];

const char *occupancyGrid[12][16];

using namespace mbed;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, MAP_PIN,
                                               NEO_TILE_BOTTOM + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                                                   NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_BGR + NEO_KHZ800);

void mapSetup()
{
    matrix.begin();
    matrix.show();
    matrix.setTextWrap(false);
    matrix.setBrightness(10);
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.setCursor(0, 0);
    mapReset();
    matrix.show();
}

void mapReset()
{
    matrix.fillScreen(OFF);
    resetGrid();
    mapDrawBoundary();
    previousLocation[0] = 3;
    previousLocation[1] = 14;
    currentLocation[0] = 3;
    currentLocation[1] = 14;
    matrix.drawPixel(currentLocation[0], currentLocation[1], BLUE);
    matrix.show();
}

void mapOverwriteLocation(int newLocation[2])
{
    currentLocation[0] = newLocation[0];
    currentLocation[1] = newLocation[1];
    mapUpdate();
}

void mapUpdate(void)
{
    matrix.drawLine(previousLocation[0], previousLocation[1], currentLocation[0], currentLocation[1], RED);
    matrix.drawPixel(currentLocation[0], currentLocation[1], BLUE);
    matrix.show();
    
    //updateGrid(currentLocation[0], currentLocation[1], "O");
    //updateGrid(previousLocation[0], previousLocation[1], "O");

    for (int y = currentLocation[1]; y <= previousLocation[1]; y++)
    {
        updateGrid(currentLocation[0], y, "O");
    }
}

void mapUpdateLocation(int distance)
{
    int angle = motorControl.getCurrentAngle();
    if (angle == 0)
    {
        currentLocation[1] -= distance / 100;
    }
    else if (angle == 90)
    {
        currentLocation[0] -= distance / 100;
    }
    else if (angle == 180)
    {
        currentLocation[1] += distance / 100;
    }
    else if (angle == 270)
    {
        currentLocation[0] += distance / 100;
    }
    else
    {
    }
    mapUpdate();
    previousLocation[0] = currentLocation[0];
    previousLocation[1] = currentLocation[1];
}

int getCurrentX(void)
{
    return currentLocation[1];
}
int getCurrentY(void)
{
    return currentLocation[0];
}

void errorScreen(void)
{
    matrix.fillScreen(RED);
}

void drawObstacle(void)
{
    matrix.drawPixel(currentLocation[0], currentLocation[1] - 1, YELLOW);
    matrix.show();

    updateGrid(currentLocation[0], currentLocation[1] - 1, "X");
}

void resetGrid(void)
{
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 12; x++)
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
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 12; x++)
        {
            Serial.print(occupancyGrid[x][y]);
        }
        Serial.println("");
    }
}

void mapDrawBoundary()
{
    matrix.drawRect(2, 0, 12, 16, GREEN);
}
