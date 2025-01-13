#ifndef MAP_H
#define MAP_H

#include "mbed.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"

#define MAP_PIN A7

#define OFF matrix.Color(0,0,0)
#define WHITE matrix.Color(255,255,255)
#define RED matrix.Color(0,255,0)
#define GREEN matrix.Color(0,0,255)
#define BLUE matrix.Color(255,0,0)
#define YELLOW matrix.Color(0,255,255)

using namespace mbed;

void mapSetup(void);
void mapReset(void);
void mapOverwriteLocation(int newLocation[2]);
void mapUpdate(void);
void mapDrawBoundary(void);
void mapUpdateLocation(int distance);
int getCurrentX(void);
int getCurrentY(void);
void errorScreen(void);
void drawObstacle(void);
void resetGrid(void);
void updateGrid(int x, int y, char object);
void printGrid();

#endif
