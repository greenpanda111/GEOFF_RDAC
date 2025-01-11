#include "mbed.h"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_GFX.h"
#include "MotorController.h"
#include "LED.h"
#include "Ultrasonics.h"
#include "IR.h"
#include "NESControllerInterface.h"
#include "Encoders.h"
#include "NES.h"
#include "Neopixel.h"
#include "rtos.h"
#include "Bumpers.h"
#include "Map.h"
#include "MazeSolver.h"

using namespace mbed;

#define STD_DELAY 100000

Ultrasonic leftUltrasonic(LEFT_ULTRASONIC_SENSOR_PIN);
Ultrasonic rightUltrasonic(RIGHT_ULTRASONIC_SENSOR_PIN);

int ultrasonicOutputList[2] = {0, 0};

/*
void sensorsOutput()
{

  IROutputList[0] = frontLeftIR.read();
  IROutputList[1] = frontRightIR.read();
  IROutputList[2] = sideLeftIR.read();
  IROutputList[3] = sideRightIR.read();

  Serial.print("IR:");
  for (int i = 0; i < 4; i++)
  {
    Serial.print(IROutputList[i]);
    if (i < 3)
    {
      Serial.print(",");
    }
  }
  Serial.println("");

  ultrasonicOutputList[0] = leftUltrasonic.read();
  ultrasonicOutputList[1] = rightUltrasonic.read();

  Serial.print("ultrasonic:");
  Serial.print(ultrasonicOutputList[0]);
  Serial.print(",");
  Serial.print(ultrasonicOutputList[1]);
  Serial.println("");
}
*/
void setup()
{
  // sensorThread.start(sensorThread);
  leftUltrasonic.correction();
  rightUltrasonic.correction();

  motorControl.setup();
  mapSetup();
  mazeSolverSetup();
}

void loop()
{
  Serial.println("Loop Start --------------------------------");

  // matrix.drawBitmap(0,0,bitmap,16,16,matrix.Color(0,255,255));
  // matrix.drawPixel(0,0,matrix.Color(255,255,255));

  //IRAveraging();
  frontIRAveraging();
  frontIROutput();
  solveMaze();

  wait_us(STD_DELAY);

  Serial.println();
}