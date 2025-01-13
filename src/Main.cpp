#include "mbed.h"
#include "Motioncontroller.h"
#include "Map.h"
#include "MazeSolver.h"

using namespace mbed;

#define STD_DELAY 100000

void setup()
{
  motorControl.setup();
  mapSetup();
  resetGrid();
  mazeSolverSetup();
}

void loop()
{
  Serial.println("Loop Start --------------------------------");

  // matrix.drawBitmap(0,0,bitmap,16,16,matrix.Color(0,255,255));
  // matrix.drawPixel(0,0,matrix.Color(255,255,255));

  IRAveraging();
  IROutput();
  // solveMaze();
  printGrid();

  wait_us(STD_DELAY);

  Serial.println();
}