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

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, A7,
                                               NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_BGR + NEO_KHZ800);


const unsigned char PROGMEM bitmap[] ={

     0xff, 0xff, 0x9e, 0x19, 0x90, 0x11, 0x90, 0x71, 0x80, 0x1, 0x80, 0x1, 
  0x84, 0x1, 0x84, 0x1, 0x84, 0x1, 0x84, 0x1, 0x86, 0x71, 0x90, 0x41, 
  0x90, 0x41, 0x90, 0x41, 0x93, 0x41, 0xff, 0xff, 
};

int x = matrix.width();

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
  // thread.start(sensorThread);
  leftMotor.setup();
  rightMotor.setup();
  leftUltrasonic.correction();
  rightUltrasonic.correction();
  leftEncoder.setup();
  rightEncoder.setup();
  leftEncoder.reset();
  rightEncoder.reset();

  // matrixSetup(matrix);

  matrix.begin();
  matrix.show();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.setTextColor(matrix.Color(0, 0, 255));
  matrix.setCursor(0, 0);
}

void loop()
{
  Serial.println("Loop Start --------------------------------");
  
  //matrix.fillScreen(matrix.Color(255,0,0));
  //matrix.show();
  
  // printScroll(matrix, "aaaaabbbb", x);

 
  //matrix.drawLine(1,0,1,16,matrix.Color(255,0,0));
  //matrix.drawLine(0,1,16,1,matrix.Color(255,0,0));
  //matrix.drawLine(1,0,16,10,matrix.Color(255,0,0));
  //matrix.drawCircle(8,8,6,matrix.Color(0,255,0));
  matrix.drawBitmap(0,0,bitmap,16,16,matrix.Color(255,255,255));
  matrix.drawPixel(0,0,matrix.Color(255,0,0));
  matrix.show();
/*
  matrix.setCursor(x, 0);
  matrix.print(F("abcd"));
  if (--x < -36)
  {
    x = matrix.width();
  }
  matrix.show();
  wait_us(200000);
*/
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
  // motorControl.forward();
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
  // sensorsOutput();

  Serial.println();
}