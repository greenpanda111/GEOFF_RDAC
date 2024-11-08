#include "mbed.h"
#include "Arduino.h"
#include "MotorController.h"
#include "LED.h"
#include "Ultrasonics.h"
#include "IR.h"
#include "NESControllerInterface.h"
#include "Encoders.h"
#include "NES.h"
#include "Adafruit_NeoPixel.h"

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

Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Adafruit_NeoPixel strip(64, A7, NEO_BGR + NEO_KHZ800);

rtos::Thread thread;

int IROutputList[4] = {0, 0, 0, 0};
int ultrasonicOutputList[2] = {0, 0};

void sensorThread()
{
  while (true)
  {
    IROutputList[0] = frontLeftIR.read();
    IROutputList[1] = frontRightIR.read();
    IROutputList[2] = sideLeftIR.read();
    IROutputList[3] = sideRightIR.read();

    ultrasonicOutputList[0] = leftUltrasonic.read();
    ultrasonicOutputList[1] = rightUltrasonic.read();
    Serial.println("Thread loop=====================");
  }
}

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

void setup()
{
  //thread.start(sensorThread);
  leftMotor.setup();
  rightMotor.setup();
  leftUltrasonic.correction();
  rightUltrasonic.correction();
  leftEncoder.setup();
  rightEncoder.setup();
  leftEncoder.reset();
  rightEncoder.reset();

  strip.begin();
  strip.show();
  strip.setBrightness(32);
}

void loop()
{
  Serial.println("Loop Start --------------------------------");

  for(int i=0;i<65;i++){
    strip.setPixelColor(i,0,0,255);
  }
  strip.show();

  /*
  Serial.print("left dist: ");
  Serial.print(leftEncoder.getForwardDist());
  Serial.println("mm");
  leftEncoder.reset();
  wait_us(STD_DELAY);
  */

  // NES();
  // led.cycle();

  led.state(GREEN);
  //motorControl.forward();
  /*
  while (ultrasonicOutputList[0] < 7)
  {
    motorControl.reverse();
    led.state(RED);
    wait_us(STD_DELAY);
    motorControl.rotateClockwise();
    led.state(BLUE);
    wait_us(STD_DELAY);
  }
  motorControl.stop();
  */
  //sensorsOutput();

  Serial.println();
}