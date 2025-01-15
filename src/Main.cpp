#include "mbed.h"
#include "Motioncontroller.h"
#include "Map.h"
#include "MazeSolver.h"

using namespace mbed;

#define STD_DELAY 100000

void setup()
{
  // Set up the motor controller and motors
  motorControl.setup();
  // Initialise the LED map and occupancy grid
  mapSetup();
  // Reset robot position and set starting state
  mazeSolverSetup();
}

void loop()
{
  Serial.println("Loop Start --------------------------------");

  solveMaze();

  wait_us(STD_DELAY);

  Serial.println();
}