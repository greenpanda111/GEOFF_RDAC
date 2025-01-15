
#ifndef MotionController_h
#define MotionController_h
#include "MotorClass.h"
#include "mbed.h"

// pins for left and right encoders
// use of both A and B pin of each encoder is achieved by using GPIO that would otherwise be connected to ultrasonics
#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

using namespace mbed;

class MotionController
{
public:
  MotionController(Motor &leftMotor, Motor &rightMotor);
  // moves the robot forward a given distance in mm
  void forwardDist(int distance);
  // moves the robot backwards in a given distance in mm
  void reverseDist(int distance);
  // turns the robot a given angle in degrees
  void rotate(int angle);
  // stops the robot moving
  void stop();
  // sets up the motors and bumper, as well as initialising the angle and alignment flag variable
  void setup();
  // returns the current angle the robot is facing in degrees
  int getCurrentAngle();
  // overwrites the current angle of the robot
  void setCurrentAngle(int);
  // flag to set high when the bumpers ISR is run
  void setStuck(bool status);
  // flag to set high when the robot is aligning to a wall
  void setAlign(bool status);

private:
  // individual motor objects
  Motor &_leftMotor;
  Motor &_rightMotor;

  // calclates the distance the motors must move to turn a given angle
  float arcLength(float angle);
  // increases the angle of the robot by a give amount, capping it between 0 and 360 degrees
  void updateCurrentAngle(int angleChange);
  // triggered if setStuck flag is set high by the bumper when trying to move a distance
  // Causes the robot to reverse and save the location of the obstacle it hit
  void avoid(void);

  // stores the current angle of the robot
  int _currentAngle;
  // flag that goes high when the bumpers are hit
  bool _stuck;
  // flag that goes high when aligning to a wall
  bool _align;
};
extern MotionController motorControl;

#endif // MOTORCONTROLLER_H
