#include "mbed.h"
#include "Arduino.h"
#include "MotorController.h"
#include "LED.h"
#include "Ultrasonics.h"
#include "IR.h"
#include "NESControllerInterface.h"

using namespace mbed;

#define STD_DELAY 830000

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction);

MotorController motorControl(leftMotor, rightMotor);

Ultrasonic leftUltrasonic(LEFT_ULTRASONIC_SENSOR_PIN);
Ultrasonic rightUltrasonic(RIGHT_ULTRASONIC_SENSOR_PIN);

IR frontLeftIR(LEFT_FRONT);
IR frontRightIR(RIGHT_FRONT);
IR sideLeftIR(LEFT_SIDE);
IR sideRightIR(RIGHT_SIDE);

LED led(GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4);

// NESControllerInterface nes(A6, A3, A2); // GPIO 2,3,4

int IROutputList[4] = {0, 0, 0, 0};
int ultrasonicOutputList[2] = {0, 0};

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
/*
void NES()
{
  NESInput input = nes.getNESInput();

  if (input.buttonUp == true)
  {
    motorControl.reverse();
    wait_us(STD_DELAY * 0.01);
  }
  if (input.buttonDown == true)
  {
    motorControl.forward();
    wait_us(STD_DELAY * 0.01);
  }
  if (input.buttonLeft == true)
  {
    motorControl.rotateCounterClockwise();
    wait_us(STD_DELAY * 0.01);
  }
  if (input.buttonRight == true)
  {
    motorControl.rotateClockwise();
    wait_us(STD_DELAY * 0.01);
  }
  motorControl.stop();
}
*/
void setup()
{
  leftMotor.setup();
  rightMotor.setup();
  leftUltrasonic.correction();
  rightUltrasonic.correction();
}

void loop()
{
  Serial.println("Loop Start --------------------------------");
  // NES();
  //    led.cycle();
  led.state(GREEN);

  //motorControl.forward();

  while (leftUltrasonic.read() < 7)
  {
    motorControl.reverse();
    led.state(RED);
    wait_us(STD_DELAY);
    motorControl.rotateClockwise();
    led.state(BLUE);
    wait_us(STD_DELAY);
  }
  motorControl.stop();

   sensorsOutput();

  Serial.println();
}