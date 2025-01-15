
#include "Arduino.h"
#include "MotorClass.h"
#include "MotionController.h"
#include "mbed.h"
#include "Map.h"
#include "Bumpers.h"

using namespace mbed;

// proportional constant
#define KP 0.5
// default speed of the motors (%)
#define DEFAULTSPEED 0.4
// distance from the centre of the robot to the wheels
#define RADIUS 53
// motors are opposite so must turn in opposite directions for forward motion
#define LEFT_FORWARD 0.5
#define RIGHT_FORWARD -0.5

// build encoder objects for each motor
Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

// build motors and pass in encoder objects
Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction, LEFT_FORWARD, leftEncoder);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction, RIGHT_FORWARD, rightEncoder);

// build motorController object with motors
MotionController motorControl(leftMotor, rightMotor);

// build bumpers
Bumper bumper(GPIO_PIN_3, GPIO_PIN_2);

MotionController::MotionController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}

void MotionController::setup()
{
  // run setup functions for motors and bumper
  _leftMotor.setup();
  _rightMotor.setup();
  bumper.setup();
  // set angle to face the finish
  _currentAngle = 0;
  // set alignment flag low
  _align = false;
}

int MotionController::getCurrentAngle()
{
  return _currentAngle;
}

void MotionController::setCurrentAngle(int angle)
{
  _currentAngle = angle;
}

void MotionController::setStuck(bool status)
{
  _stuck = status;
}

void MotionController::setAlign(bool status)
{
  _align = status;
}

void MotionController::forwardDist(int distance)
{
  // reset encoder counts to 0
  leftEncoder.reset();
  rightEncoder.reset();

  // while the robot has not moved the distance given and the bumpers have not been activated
  while ((abs(leftEncoder.getDistance()) < distance) & (abs(rightEncoder.getDistance()) < distance) & (_stuck == false))
  {
    // calculate error between encoders and multiply by proportional constant
    int error = (leftEncoder.getDistance() - rightEncoder.getDistance()) * KP;

    // alter the speed of each motor according to the error to keep the robot as straight as possible
    _leftMotor.move(DEFAULTSPEED + error);
    _rightMotor.move(-(DEFAULTSPEED - error));
  }

  // stop robot if distance is reached or obstacle hit
  stop();

  // if obstacle is hit
  if (_stuck == true)
  {
    // update location with the distance the encoders got to when collision occured
    mapUpdateLocation((leftEncoder.getDistance()+rightEncoder.getDistance())/2);
    // move away from the obstacle
    motorControl.avoid();
  }
  else
  {
    // update location with the distance travelled
    mapUpdateLocation(distance);
  }
}

void MotionController::reverseDist(int distance)
{
  // reset encoder counts to 0
  leftEncoder.reset();
  rightEncoder.reset();

  // while the robot has not moved the distance given
  while ((abs(leftEncoder.getDistance()) < distance) & (abs(rightEncoder.getDistance()) < distance))
  {
    // calculate error between encoders and multiply by proportional constant
    int error = (leftEncoder.getDistance() - rightEncoder.getDistance()) * KP;

    // alter the speed of each motor according to the error to keep the robot as straight as possible
    _leftMotor.move(-(DEFAULTSPEED - error));
    _rightMotor.move((DEFAULTSPEED + error));
  }

  // stop robot if distance is reached
  stop();
  mapUpdateLocation(-distance);
  
}

void MotionController::rotate(int angle)
{
  // reset encoder counts to 0
  leftEncoder.reset();
  rightEncoder.reset();
  // increase the robots angle by the given angle
  updateCurrentAngle(angle);

  // calculate distance for the motors to turn for rotation of the robot to reach the given angle
  float distance = arcLength(abs(angle));

  // if rotating counter clockwise
  if (angle < 0)
  {
    // while the motors have not turned the distance given
    while ((abs(leftMotor.getEncoderDist()) < distance) & (abs(rightMotor.getEncoderDist()) < distance))
    {
      // calculate error and multiply by proportional constant
      int error = (leftEncoder.getDistance() + rightEncoder.getDistance()) * KP;

      // alter the speed of each motor to keep them turning at the same rate
      _leftMotor.move((DEFAULTSPEED - error));
      _rightMotor.move((DEFAULTSPEED + error));
    }
  }
  // if rotating clockwise
  else
  {
    // while the motors have not turned the distance given
    while ((abs(leftMotor.getEncoderDist()) < distance) & (abs(rightMotor.getEncoderDist()) < distance))
    {
      // calculate error and multiply by proportional constant
      int error = (leftEncoder.getDistance() + rightEncoder.getDistance()) * KP;

      // alter the speed of each motor to keep them turning at the same rate
      _leftMotor.move(-(DEFAULTSPEED + error));
      _rightMotor.move(-(DEFAULTSPEED - error));
    }
  }

  // stop robot if distance to turn by is reached
  stop();
}

float MotionController::arcLength(float angle)
{
  // calculate arc length using equation
  // convert angle to radians
  float arcLength = (2 * PI * RADIUS * (angle / 360));
  return arcLength;
}

void MotionController::updateCurrentAngle(int angleChange)
{
  // if rotate function is called by the wall alignment function, don't change the angle stored
  if (_align == false)
  {
    // add the change in angle to the current angle
    _currentAngle += angleChange;

    // if the result is larger than 360
    if (_currentAngle >= 360)
    {
      // loop back round to zero and continue adding from there
      _currentAngle = _currentAngle - 360;
    }
    // if the result goes negative
    else if (_currentAngle < 0)
    {
      // loop back to 360 and continute subtracting from there
      _currentAngle = 360 - abs(_currentAngle);
    }
  }
  else
  {
  }
}

void MotionController::avoid(void)
{
  // reset stuck flag
  motorControl.setStuck(false);
  // stop motors from spinning
  motorControl.stop();
  // save the location of the obstacle in front
  drawObstacle();
  // reverse away from the obstacle
  motorControl.reverseDist(100);
}

void MotionController::stop()
{
  _leftMotor.stop();
  _rightMotor.stop();
}