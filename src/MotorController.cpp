
#include "Arduino.h"
#include "MotorClass.h"
#include "MotorController.h"
#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

Encoder _leftEncoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder _rightEncoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

Motor leftMotor(Left_Motor_PWM, Left_Motor_Direction);
Motor rightMotor(Right_Motor_PWM, Right_Motor_Direction);

MotorController motorControl(leftMotor, rightMotor);

MotorController::MotorController(Motor &leftMotor, Motor &rightMotor)
    : _leftMotor(leftMotor), _rightMotor(rightMotor)
{
}
void MotorController::setup()
{
  _leftMotor.setup();
  _rightMotor.setup();
  _leftEncoder.setup();
  _rightEncoder.setup();
  _pwr = Default_Power;
  _currentAngle = 0;
}
int MotorController::getCurrentAngle()
{
  return _currentAngle;
}
void MotorController::setCurrentAngle(int angle)
{
  _currentAngle = angle;
}
void MotorController::forward(int distance)
{

  _leftEncoder.reset();
  while (_leftEncoder.getDistance() < distance)
  {
    Serial.print("encoder Dist: ");
    Serial.println(_leftEncoder.getDistance());

    _leftMotor.move(Right_Forward, _pwr);
    _rightMotor.move(Left_Forward, _pwr);
  }
  _leftMotor.stop();
  _rightMotor.stop();
}

void MotorController::reverse(int distance)
{

  _leftEncoder.reset();
  while (_leftEncoder.getDistance() > -distance)
  {
    Serial.print("encoder Dist: ");
    Serial.println(_leftEncoder.getDistance());

    _leftMotor.move(!Right_Forward, _pwr);
    _rightMotor.move(!Left_Forward, _pwr);
  }
  _leftMotor.stop();
  _rightMotor.stop();
}

void MotorController::rotate(int angle)
{
  _leftEncoder.reset();
  
  if (angle>0){
  while (-_leftEncoder.getDistance() < arcLength(angle))
  {
    Serial.print(_leftEncoder.getDistance());
    Serial.print(" of ");
    Serial.println(arcLength(angle));
    _leftMotor.move(Left_Forward, _pwr);
    _rightMotor.move(!Right_Forward, _pwr);
  }
  }
  else{
     while (_leftEncoder.getDistance() > -arcLength(angle))
  {
    _leftMotor.move(!Left_Forward, _pwr);
    _rightMotor.move(Right_Forward, _pwr);
  }
  }
  _leftMotor.stop();
  _rightMotor.stop();
  updateCurrentAngle(angle);

}


float MotorController::arcLength(float angle)
{
  float arcLength = (2 * 3.14 * 48*(angle / 360));
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
void MotorController::stop()
{
  _leftMotor.move(Left_Forward, 0.0f);
  _rightMotor.move(Right_Forward, 0.0f);
}