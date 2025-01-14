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
  mazeSolverSetup();
}

void loop()
{
  Serial.println("Loop Start --------------------------------");

  IRAveraging();
  IROutput();
  //solveMaze();

  wait_us(STD_DELAY);

  Serial.println();
}