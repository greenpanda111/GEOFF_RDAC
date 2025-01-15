
#ifndef MotionController_h
#define MotionController_h
#include "MotorClass.h"
#include "mbed.h"

// Pins for left and right encoders
// Use of both A and B pin of each encoder is achieved by using GPIO that would otherwise be connected to ultrasonics
#define LEFT_ENCODER_A P1_11
#define LEFT_ENCODER_B P1_15
#define RIGHT_ENCODER_A P1_12
#define RIGHT_ENCODER_B P1_14

using namespace mbed;

class MotionController
{
public:
  MotionController(Motor &leftMotor, Motor &rightMotor);
  // Moves the robot forward a given distance in mm
  void forwardDist(int distance);
  // Moves the robot backwards in a given distance in mm
  void reverseDist(int distance);
  // Turns the robot a given angle in degrees
  void rotate(int angle);
  // Stops the robot moving
  void stop();
  // Sets up the motors and bumper, as well as initialising the angle and alignment flag variable
  void setup();
  // Returns the current angle the robot is facing in degrees
  int getCurrentAngle();
  // Overwrites the current angle of the robot
  void setCurrentAngle(int);
  // Flag to set high when the bumpers ISR is run
  void setStuck(bool status);
  // Flag to set high when the robot is aligning to a wall
  void setAlign(bool status);

private:
  // Individual motor objects
  Motor &_leftMotor;
  Motor &_rightMotor;

  // Calclates the distance the motors must move to turn a given angle
  float arcLength(float angle);
  // Increases the angle of the robot by a give amount, capping it between 0 and 360 degrees
  void updateCurrentAngle(int angleChange);
  // Triggered if setStuck flag is set high by the bumper when trying to move a distance
  // Causes the robot to reverse and save the location of the obstacle it hit
  void avoid(void);

  // Stores the current angle of the robot
  int _currentAngle;
  // Flag that goes high when the bumpers are hit
  bool _stuck;
  // Flag that goes high when aligning to a wall
  bool _align;
};
extern MotionController motorControl;

#endif // MOTORCONTROLLER_H
