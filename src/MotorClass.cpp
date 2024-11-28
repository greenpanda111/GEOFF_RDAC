#include "Arduino.h"
#include "MotorClass.h"
#include "Encoders.h"

#include "mbed.h"
using namespace mbed;

#define DISTANCE_KP 0.07
#define DISTANCE_KI 0.00001
#define DISTANCE_KD 0.000001

#define VELOCITY_KP 0.0008
#define VELOCITY_KI 0.05
#define VELOCITY_KD 0

#define TICKER_DELAY 0.1

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection), _encoder(encoder) {}

void Motor::setup(void)
{
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
  _encoder.setup();
  _PIDSet = false;
  _PIDTicker.attach(callback(this, &Motor::PID), TICKER_DELAY);
  _velocityTicker.attach(callback(this, &Motor::calculateCurrentVelocity), TICKER_DELAY);
}

void Motor::move(float power)
{

  _PIDSet = true;
  setTargetVelocity(abs(power*500));
  if (power > 0)
  {
    _dirPin.write(_forwardDirection);
    //_PwmPin.write(power);
  }
  else
  {
    _dirPin.write(!_forwardDirection);
    //_PwmPin.write(abs(power));
  }
}

void Motor::stop(void)
{
  _PwmPin.write(0.0f);
  _PIDSet = false;
}

void Motor::calculateCurrentVelocity(void)
{
  float velocity = 0;
  velocity = _encoder.getDistance() / 0.1;
  setCurrentVelocity(velocity);
}

void Motor::resetEcoder(void)
{
  _encoder.reset();
}

float Motor::getEncoderDist(void)
{
  return _encoder.getDistance();
}

void Motor::PID()
{
  if (_PIDSet == false)
  {
    return;
  }

  float proportional = 0;
  float integral = 0;
  float derivative = 0;
  float output = 0;

  float error = _targetDistance - _currentDistance;
  // update last error
  _lastError = error;
  //constant velocity
  if (error==0){
    output=0;
  }/*
  else if (error > WHEEL_CIRCUMFERENCE){
    setTargetVelocity(500);
  }*/
  
  else if (error > WHEEL_CIRCUMFERENCE)// /8
  {
    error = _targetVelocity - _currentVelocity;
    proportional = error * VELOCITY_KP;
    integral += error * TICKER_DELAY * VELOCITY_KI;
    derivative = (error - _lastError)/TICKER_DELAY *VELOCITY_KD;
    output = (proportional + integral + derivative);
  }
  
  else
  {
    // distance
    proportional = DISTANCE_KP * error;
    integral = DISTANCE_KI * error * TICKER_DELAY;
    derivative = DISTANCE_KD * ((error - _lastError) / TICKER_DELAY);

    output = proportional + integral + derivative;
  }
  // clamp output between 0.4 and -0.4
  if (output > 0.5f)
  {
    output = 0.5f;
  }
  if (output < -0.5f)
  {
    output = -0.5f;
  }
  
  _PwmPin.write(output);
}

void Motor::setTargetDistance(float target)
{
  _targetDistance = target;
}

void Motor::setCurrentDistance(float current)
{
  _currentDistance = current;
}

void Motor::setTargetVelocity(float target)
{
  _targetVelocity = target;
}

void Motor::setCurrentVelocity(float current)
{
  _currentVelocity = current;
}

float Motor::getCurrentVelocity(void)
{
  return _currentVelocity;
}
