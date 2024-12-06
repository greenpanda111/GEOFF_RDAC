
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"

using namespace mbed;

#define LEFT_FORWARD 0.5
#define RIGHT_FORWARD -0.5
#define LEFT_REVERSE -0.5
#define RIGHT_REVERSE 0.5

Encoder leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction, LEFT_FORWARD, leftEncoder);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction, RIGHT_FORWARD, rightEncoder);

MotorController motorControl(leftMotor, rightMotor);

MotorController::MotorController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}
void MotorController::setup()
{
  _leftMotor.setup();
  _rightMotor.setup();
  _currentAngle = 0;
}
int MotorController::getCurrentAngle()
{
  return _currentAngle;
}

void MotorController::setStuck(bool status)
{
  _stuck = status;
}

void MotorController::forwardDist(int distance)
{
  _leftMotor.resetEncoder();
  _rightMotor.resetEncoder();

  _leftMotor.setTargetDistance(distance);
  _rightMotor.setTargetDistance(distance);

  while ((_leftMotor.getEncoderDist() < distance) & (_rightMotor.getEncoderDist() < distance) & (_stuck == false))
  {
    Serial.print(leftMotor.getEncoderDist());
    Serial.print(" of ");
    Serial.println(distance);
    // Serial.println(leftMotor.getCurrentVelocity());
    _leftMotor.setCurrentDistance(_leftMotor.getEncoderDist());
    _rightMotor.setCurrentDistance(_rightMotor.getEncoderDist());

    _leftMotor.move(LEFT_FORWARD);
    _rightMotor.move(RIGHT_FORWARD);
  }
  _leftMotor.stop();
  _rightMotor.stop();

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotorController::reverseDist(int distance)
{
  _leftMotor.resetEncoder();
  _rightMotor.resetEncoder();

  _leftMotor.setTargetDistance(distance);
  _rightMotor.setTargetDistance(distance);
  Serial.println("reverse");

  while ((_leftMotor.getEncoderDist() > -distance) & (_rightMotor.getEncoderDist() > -distance) & (_stuck == false))
  {
    Serial.print(leftMotor.getEncoderDist());
    Serial.print(" of ");
    Serial.println(distance);

    _leftMotor.setCurrentDistance(abs(_leftMotor.getEncoderDist()));
    _rightMotor.setCurrentDistance(abs(_rightMotor.getEncoderDist()));

    _leftMotor.move(LEFT_REVERSE);
    _rightMotor.move(RIGHT_REVERSE);
  }
  _leftMotor.stop();
  _rightMotor.stop();

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

void MotorController::rotate(int angle)
{
  leftMotor.resetEncoder();
  rightMotor.resetEncoder();
  Serial.println("rotating");
  Serial.println(arcLength(angle));

  leftMotor.setIsRotating(true);
  rightMotor.setIsRotating(true);

  float distance = arcLength(angle);

  _leftMotor.setTargetDistance(distance);
  _rightMotor.setTargetDistance(distance);

  if (angle > 0)
  {
    while ((abs(leftMotor.getEncoderDist()) < distance) & (abs(rightMotor.getEncoderDist()) < distance) & (_stuck == false))
    {
      Serial.print(rightMotor.getEncoderDist());
      Serial.print(" of ");
      Serial.println(distance);

      _leftMotor.setCurrentDistance(abs(_leftMotor.getEncoderDist()));
      _rightMotor.setCurrentDistance(abs(_rightMotor.getEncoderDist()));

      _leftMotor.move(LEFT_FORWARD);
      _rightMotor.move(RIGHT_REVERSE);
    }
  }
  if (angle < 0)
  {
    while ((leftMotor.getEncoderDist() > -arcLength(angle)) & (rightMotor.getEncoderDist() < arcLength(angle)) & (_stuck == false))
    {
      _leftMotor.move(LEFT_REVERSE);
      _rightMotor.move(RIGHT_FORWARD);
    }
  }
  else
  {
  }

  _leftMotor.stop();
  _rightMotor.stop();
  updateCurrentAngle(angle);

  leftMotor.setIsRotating(false);
  leftMotor.setIsRotating(false);

  if (_stuck == true)
  {
    motorControl.avoid();
  }
}

float MotorController::arcLength(float angle)
{
  float arcLength = (2 * 3.14 * 49.5 * (angle / 360));
  return arcLength;
}
void MotorController::updateCurrentAngle(int angleChange)
{
  _currentAngle += angleChange;
  if (_currentAngle > 360)
  {
    _currentAngle = _currentAngle - 360;
  }
}

void MotorController::avoid(void)
{
  Serial.println("avoiding");
  motorControl.setStuck(false);
  motorControl.stop();
  motorControl.reverseDist(100);
  motorControl.rotate(90);
}

void MotorController::forwardVelocity(int velocity)
{
  _leftMotor.setTargetVelocity(velocity);
  _rightMotor.setTargetVelocity(velocity);
}

void MotorController::reverseVelocity(int velocity)
{
  _leftMotor.setTargetVelocity(velocity);
  _rightMotor.setTargetVelocity(velocity);
}

void MotorController::stop()
{
  _leftMotor.stop();
  _rightMotor.stop();
}